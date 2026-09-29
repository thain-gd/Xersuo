// Fill out your copyright notice in the Description page of Project Settings.


#include "NormalAttack.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Animation/AnimMontage.h"
#include "Champion/NormalAttackConfig.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Xersuo.h"
#include "XersuoCharacter.h"
#include "XersuoPlayerState.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystem/AttributeSets/BaseStatsAttributeSet.h"
#include "AbilitySystem/GameplayEffects/DamageEffect.h"
#include "AbilitySystem/GameplayTags/XersuoGameplayTags.h"
#include "Kismet/GameplayStatics.h"
#include "Projectile/AttackTargetInterface.h"
#include "Projectile/TargetedProjectile.h"

UNormalAttack::UNormalAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;
	NetSecurityPolicy = EGameplayAbilityNetSecurityPolicy::ServerOnly;
}

void UNormalAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                    const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                    const FGameplayEventData* TriggerEventData)
{
	AttackTarget = TriggerEventData ? TriggerEventData->Target.Get() : nullptr;
	if (!AttackTarget.IsValid())
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	const AXersuoPlayerState* ChampionState = ActorInfo
		? Cast<AXersuoPlayerState>(ActorInfo->OwnerActor.Get()) : nullptr;
	AttackConfig = ChampionState ? ChampionState->GetNormalAttackConfig() : nullptr;
	if (!AttackConfig)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	// For now, only 1 attack montage
	UAnimMontage* AttackMontage = IsValid(AttackConfig->AttackMontages[0]) ? AttackConfig->AttackMontages[0].Get() : nullptr;
	if (!AttackMontage || !ActorInfo || !ActorInfo->GetAnimInstance())
	{
		UE_LOG(LogXersuo, Warning, TEXT("NormalAttack cannot play: missing attack configuration, valid montage, or avatar AnimInstance for %s."),
			*GetNameSafe(ChampionState));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	// ServerInitiated abilities also execute here on the owning client. Face the target
	// before either machine starts the montage, without waiting for rotation replication.
	if (ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get()))
	{
		if (APlayerController* Controller = ActorInfo->PlayerController.Get())
		{
			Controller->StopMovement();
		}
		Character->ConsumeMovementInputVector();
		Character->GetCharacterMovement()->StopMovementImmediately();
		const FVector Direction = (AttackTarget->GetActorLocation() - Character->GetActorLocation()).GetSafeNormal2D();
		if (!Direction.IsNearlyZero())
		{
			Character->SetActorRotation(FRotator(0.f, Direction.Rotation().Yaw, 0.f));
		}
	}

	if (AttackConfig->Delivery == ENormalAttackDelivery::Projectile)
	{
		UAbilityTask_WaitGameplayEvent* EventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, XersuoGameplayTags::Event_Attack_ProjectileLaunch);
		EventTask->EventReceived.AddDynamic(this, &UNormalAttack::OnProjectileLaunchEvent);

		EventTask->ReadyForActivation();
	}

	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this, TEXT("NormalAttackMontage"), AttackMontage, 0.65f, NAME_None,
		true, 1.f, 0.f, true);
	if (!MontageTask)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	MontageTask->OnCompleted.AddDynamic(this, &UNormalAttack::OnAttackMontageCompleted);
	MontageTask->OnInterrupted.AddDynamic(this, &UNormalAttack::OnAttackMontageCancelled);
	MontageTask->OnCancelled.AddDynamic(this, &UNormalAttack::OnAttackMontageCancelled);

	MontageTask->ReadyForActivation();
}

void UNormalAttack::OnAttackMontageCompleted()
{
	if (IsActive())
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
}

void UNormalAttack::OnAttackMontageCancelled()
{
	if (IsActive())
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
}

void UNormalAttack::OnProjectileLaunchEvent(FGameplayEventData Payload)
{
	const FGameplayAbilityActorInfo* ActorInfo = GetCurrentActorInfo();
	AXersuoCharacter* Source = Cast<AXersuoCharacter>(GetAvatarActorFromActorInfo());
	if (!IsActive() || !ActorInfo || !ActorInfo->IsNetAuthority() || !IsValid(Source)
		|| !AttackTarget.IsValid() || !AttackConfig || !AttackConfig->ProjectileClass)
	{
		return;
	}

	FGameplayEffectSpecHandle DamageSpec = CreateDamageSpec();
	if (!DamageSpec.IsValid())
	{
		return;
	}

	SpawnProjectile(Source, DamageSpec);
}

FGameplayEffectSpecHandle UNormalAttack::CreateDamageSpec() const
{
	FGameplayEffectSpecHandle DamageSpec;
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
	{
		DamageSpec = MakeOutgoingGameplayEffectSpec(UDamageEffect::StaticClass());
		const float Damage = FMath::Max(0.f, ASC->GetNumericAttribute(UBaseStatsAttributeSet::GetAttackDamageAttribute()));
		DamageSpec.Data->SetSetByCallerMagnitude(XersuoGameplayTags::Data_HealthDelta, -Damage);
	}

	return DamageSpec;
}

void UNormalAttack::SpawnProjectile(AXersuoCharacter* Source, const FGameplayEffectSpecHandle& DamageSpec) const
{
	FVector SpawnLocation = Source->GetMesh()->GetSocketLocation(AttackConfig->ProjectileSpawnSocket);
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SpawnLocation);

	ATargetedProjectile* Projectile =
		GetWorld()->SpawnActorDeferred<ATargetedProjectile>(
			AttackConfig->ProjectileClass,
			SpawnTransform,
			Source,
			Source,
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn
		);

	if (!Projectile)
	{
		return;
	}
	Projectile->Init(AttackTarget.Get(), DamageSpec);
	UGameplayStatics::FinishSpawningActor(
		Projectile,
		SpawnTransform
	);
}

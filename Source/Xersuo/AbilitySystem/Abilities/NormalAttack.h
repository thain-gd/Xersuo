// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "NormalAttack.generated.h"

class AXersuoCharacter;
class UNormalAttackConfig;
class ACharacter;

/** Plays the champion's configured normal-attack montage through GAS. */
UCLASS()
class XERSUO_API UNormalAttack : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UNormalAttack();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	                             const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;

private:
	UFUNCTION()
	void OnAttackMontageCompleted();

	UFUNCTION()
	void OnAttackMontageCancelled();
	
	UFUNCTION()
	void OnProjectileLaunchEvent(FGameplayEventData Payload);
	
	FGameplayEffectSpecHandle CreateDamageSpec() const;
	void SpawnProjectile(AXersuoCharacter* Source, const FGameplayEffectSpecHandle& DamageSpec) const;
	
	const UNormalAttackConfig* AttackConfig = nullptr;
	TWeakObjectPtr<const AActor> AttackTarget;
};

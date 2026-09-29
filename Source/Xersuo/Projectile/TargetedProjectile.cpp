// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetedProjectile.h"

#include "AbilitySystemComponent.h"
#include "AttackTargetInterface.h"
#include "XersuoCharacter.h"
#include "Components/SphereComponent.h"
#include "Engine/World.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ATargetedProjectile::ATargetedProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics;
	ProjectileMovement->bTickBeforeOwner = false;
	// Calculate this frame's velocity before the movement component consumes it.
	ProjectileMovement->AddTickPrerequisiteActor(this);
}

void ATargetedProjectile::Init(const AActor* Target, const FGameplayEffectSpecHandle& InDamageSpec)
{
	TargetActor = TWeakInterfacePtr<const IAttackTargetInterface>(Target);
	DamageSpec = InDamageSpec;
}

// Called when the game starts or when spawned
void ATargetedProjectile::BeginPlay()
{
	Super::BeginPlay();
	// Old Blueprint defaults can retain the movement-before-owner dependency after a C++ change.
	ProjectileMovement->bTickBeforeOwner = false;
	RemoveTickPrerequisiteComponent(ProjectileMovement);
	ProjectileMovement->AddTickPrerequisiteActor(this);
	if (!HasAuthority())
	{
		// Clients display the server's replicated transform instead of steering independently.
		ProjectileMovement->Deactivate();
		SetActorTickEnabled(false);
		Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		return;
	}

	if (!TargetActor.IsValid())
	{
		Destroy();
		return;
	}

	// Targeted attacks pass through unrelated actors; the swept query below tests only the selected target.
	Collision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Collision->SetCollisionResponseToAllChannels(ECR_Overlap);
	Collision->OnComponentBeginOverlap.AddDynamic(this, &ATargetedProjectile::OnTargetOverlap);
	Collision->IgnoreActorWhenMoving(GetOwner(), true);
	Collision->IgnoreActorWhenMoving(GetInstigator(), true);
	ProjectileMovement->SetUpdatedComponent(Collision);
	ProjectileMovement->ProjectileGravityScale = 0.f;
	ProjectileMovement->bIsHomingProjectile = false;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->InitialSpeed = 0.f;
	ProjectileMovement->Activate();
}

// Called every frame
void ATargetedProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!HasAuthority())
	{
		return;
	}
	const IAttackTargetInterface* Target = TargetActor.Get();
	if (!Target)
	{
		Destroy();
		return;
	}
	if (DeltaTime <= 0.f)
	{
		return;
	}

	const FVector Start = Collision->GetComponentLocation();
	const FVector TargetLocation = Target->GetProjectileTargetLocation();
	const FVector ToTarget = TargetLocation - Start;
	const float Distance = ToTarget.Size();
	const FVector Direction = ToTarget.GetSafeNormal();
	// Clamp travel to avoid overshooting a nearby target and oscillating around its socket.
	const float TravelDistance = FMath::Min(ProjectileMovement->GetMaxSpeed() * DeltaTime, Distance);
	ProjectileMovement->Velocity = Direction * (TravelDistance / DeltaTime);
}

void ATargetedProjectile::OnTargetOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority() && OtherActor && OtherActor == TargetActor.GetObject())
	{
		UAbilitySystemComponent* TargetASC = Cast<AXersuoCharacter>(OtherActor)->GetAbilitySystemComponent();
		TargetASC->ApplyGameplayEffectSpecToSelf(*DamageSpec.Data.Get());
		Destroy();
	}
}


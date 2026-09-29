// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseProjectile.h"
#include "Xersuo.h"

#include "NiagaraComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
ABaseProjectile::ABaseProjectile()
{
	bReplicates = true;
	SetReplicateMovement(true);

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	SetRootComponent(Collision);
	Collision->InitSphereRadius(8.f);
	Collision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Collision->SetCollisionObjectType(ECC_WorldDynamic);
	Collision->SetCollisionResponseToAllChannels(ECR_Overlap);
	Collision->SetGenerateOverlapEvents(true);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->SetUpdatedComponent(Collision);
	ProjectileMovement->ProjectileGravityScale = 0.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bSweepCollision = true;
	
	ProjectileVFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ProjectileVFX"));
	ProjectileVFX->SetupAttachment(Collision);
	// Start after deferred spawning has supplied the final transform and Blueprint defaults.
	ProjectileVFX->SetAutoActivate(false);
	ProjectileVFX->SetAutoDestroy(false);
}

void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();
	if (GetNetMode() == NM_DedicatedServer)
	{
		ProjectileVFX->DeactivateImmediate();
		return;
	}

	if (!ProjectileVFX->GetAsset())
	{
		UE_LOG(LogXersuo, Warning, TEXT("Projectile %s (%s) has no Niagara System assigned to ProjectileVFX."),
			*GetName(), *GetClass()->GetName());
		return;
	}

	// Niagara runs locally on the host and on each client receiving the replicated projectile.
	ProjectileVFX->SetVisibility(true);
	ProjectileVFX->SetHiddenInGame(false);
	ProjectileVFX->SetPaused(false);
	ProjectileVFX->Activate(true);
}

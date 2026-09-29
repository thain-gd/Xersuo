// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseProjectile.h"
#include "GameplayEffectTypes.h"
#include "UObject/WeakInterfacePtr.h"
#include "TargetedProjectile.generated.h"

class IAttackTargetInterface;
class UPrimitiveComponent;

UCLASS()
class XERSUO_API ATargetedProjectile : public ABaseProjectile
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATargetedProjectile();

	/** Supply the target before FinishSpawning so server BeginPlay can start tracking it. */
	void Init(const AActor* Target, const FGameplayEffectSpecHandle& InDamageSpec);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
private:
	UFUNCTION()
	void OnTargetOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	TWeakInterfacePtr<const IAttackTargetInterface> TargetActor;
	
	UPROPERTY()
	FGameplayEffectSpecHandle DamageSpec;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ChampionAbilityBase.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class XERSUO_API UChampionAbilityBase : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UChampionAbilityBase();
	
	const FScalableFloat& GetResourceCost() const { return ResourceCost; }
	const FScalableFloat& GetCooldown() const { return Cooldown; }
	
	virtual const FGameplayTagContainer* GetCooldownTags() const override;
	virtual void ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "ChampionAbility")
	FScalableFloat ResourceCost;
	
	UPROPERTY(EditDefaultsOnly, Category = "ChampionAbility")
	FScalableFloat Cooldown{0.0f};
	
	UPROPERTY(EditDefaultsOnly, Category = "ChampionAbility")
	FGameplayTagContainer CooldownTags;
	
	
};

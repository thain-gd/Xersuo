// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMCAbilityCooldown.generated.h"

/**
 * 
 */
UCLASS()
class XERSUO_API UMMCAbilityCooldown : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	
public:
   	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
};

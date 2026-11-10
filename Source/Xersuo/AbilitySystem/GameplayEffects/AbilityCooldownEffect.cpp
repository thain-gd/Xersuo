// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityCooldownEffect.h"

#include "MMC/MMCAbilityCooldown.h"

UAbilityCooldownEffect::UAbilityCooldownEffect()
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;
	
	FCustomCalculationBasedFloat Calculation;
	Calculation.CalculationClassMagnitude = UMMCAbilityCooldown::StaticClass();
	
	DurationMagnitude = FGameplayEffectModifierMagnitude(Calculation);
}

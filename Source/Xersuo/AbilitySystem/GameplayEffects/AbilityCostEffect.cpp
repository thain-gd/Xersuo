// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityCostEffect.h"

#include "AbilitySystem/AttributeSets/ResourceAttributeSet.h"
#include "MMC/MMCAbilityCost.h"

UAbilityCostEffect::UAbilityCostEffect()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;
	
	FCustomCalculationBasedFloat CostCalculation;
	CostCalculation.CalculationClassMagnitude = UMMCAbilityCost::StaticClass();
	
	FGameplayModifierInfo ResourceModifier;
	ResourceModifier.Attribute = UResourceAttributeSet::GetResourceAttribute();
	ResourceModifier.ModifierOp = EGameplayModOp::AddBase;
	ResourceModifier.ModifierMagnitude = FGameplayEffectModifierMagnitude(CostCalculation);
	Modifiers.Add(ResourceModifier);
}

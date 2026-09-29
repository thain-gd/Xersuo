// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageEffect.h"

#include "AbilitySystem/AttributeSets/HealthAttributeSet.h"
#include "AbilitySystem/GameplayTags/XersuoGameplayTags.h"

UDamageEffect::UDamageEffect()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;
	
	FSetByCallerFloat HealthDelta;
	HealthDelta.DataTag = XersuoGameplayTags::Data_HealthDelta;
	
	FGameplayModifierInfo ModifierInfo;
	ModifierInfo.Attribute = UHealthAttributeSet::GetHealthAttribute();
	ModifierInfo.ModifierOp = EGameplayModOp::Additive;
	ModifierInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(HealthDelta);
	
	Modifiers.Add(ModifierInfo);
}

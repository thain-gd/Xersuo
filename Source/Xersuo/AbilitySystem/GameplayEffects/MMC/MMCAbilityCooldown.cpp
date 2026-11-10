// Fill out your copyright notice in the Description page of Project Settings.


#include "MMCAbilityCooldown.h"

#include "AbilitySystem/Abilities/ChampionAbilityBase.h"

float UMMCAbilityCooldown::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const UChampionAbilityBase* Ability = Cast<UChampionAbilityBase>(Spec.GetContext().GetAbilityInstance_NotReplicated());
	if (!Ability)
	{
		return 0.01f;
	}
	const float BaseCooldown = Ability->GetCooldown().GetValueAtLevel(Spec.GetLevel());
	return FMath::Max(0.01f, BaseCooldown);
}

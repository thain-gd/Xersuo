// Fill out your copyright notice in the Description page of Project Settings.


#include "MMCAbilityCost.h"

#include "AbilitySystem/Abilities/ChampionAbilityBase.h"

float UMMCAbilityCost::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const UChampionAbilityBase* Ability = Cast<UChampionAbilityBase>(Spec.GetContext().GetAbilityInstance_NotReplicated());
	if (!Ability)
	{
		return 0.0f;
	}

	// Negative because the GameplayEffect uses Add.
	const float Cost = Ability->GetResourceCost().GetValueAtLevel(Ability->GetAbilityLevel());
	return -FMath::Max(0.0f, Cost);
}

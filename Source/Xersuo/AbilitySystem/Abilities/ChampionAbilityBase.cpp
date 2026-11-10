// Fill out your copyright notice in the Description page of Project Settings.


#include "ChampionAbilityBase.h"

#include "AbilitySystem/GameplayEffects/AbilityCooldownEffect.h"
#include "AbilitySystem/GameplayEffects/AbilityCostEffect.h"

UChampionAbilityBase::UChampionAbilityBase()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	NetSecurityPolicy = EGameplayAbilityNetSecurityPolicy::ClientOrServer;
	
	CostGameplayEffectClass = UAbilityCostEffect::StaticClass();
	CooldownGameplayEffectClass = UAbilityCooldownEffect::StaticClass();
}

const FGameplayTagContainer* UChampionAbilityBase::GetCooldownTags() const
{
	return &CooldownTags;
}

void UChampionAbilityBase::ApplyCooldown(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	if (!CooldownGameplayEffectClass)
	{
		return;
	}
	
	const int32 AbilityLevel = GetAbilityLevel(Handle, ActorInfo);
	const float BaseCooldown = Cooldown.GetValueAtLevel(AbilityLevel);
	if (BaseCooldown <= 0.0f)
	{
		return;
	}
	
	FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(Handle, ActorInfo, ActivationInfo, CooldownGameplayEffectClass, AbilityLevel);
	if (!SpecHandle.IsValid())
	{
		return;
	}
	
	SpecHandle.Data->DynamicGrantedTags.AppendTags(CooldownTags);
	ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, SpecHandle);
}

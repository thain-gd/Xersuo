// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseStatsAttributeSet.h"
#include "Net/UnrealNetwork.h"

UBaseStatsAttributeSet::UBaseStatsAttributeSet()
{
	InitAttackDamage(0.f);
}

void UBaseStatsAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UBaseStatsAttributeSet, AttackDamage, COND_None, REPNOTIFY_Always);
}

void UBaseStatsAttributeSet::OnRep_AttackDamage(const FGameplayAttributeData& OldAttackDamage)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseStatsAttributeSet, AttackDamage, OldAttackDamage);
}

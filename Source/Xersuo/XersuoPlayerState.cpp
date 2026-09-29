// Copyright Epic Games, Inc. All Rights Reserved.

#include "XersuoPlayerState.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AttributeSets/HealthAttributeSet.h"
#include "AbilitySystem/AttributeSets/BaseStatsAttributeSet.h"
#include "Champion/ChampionDataRow.h"
#include "Champion/NormalAttackConfig.h"
#include "Engine/World.h"
#include "XersuoGameMode.h"
#include "Net/UnrealNetwork.h"

AXersuoPlayerState::AXersuoPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	HealthAttributeSet = CreateDefaultSubobject<UHealthAttributeSet>(TEXT("HealthAttributeSet"));
	BaseStatsAttributeSet = CreateDefaultSubobject<UBaseStatsAttributeSet>(TEXT("BaseStatsAttributeSet"));
	
	SetNetUpdateFrequency(30.f);
}

UAbilitySystemComponent* AXersuoPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

UNormalAttackConfig* AXersuoPlayerState::GetNormalAttackConfig() const
{
	return NormalAttackConfig.Get();
}

void AXersuoPlayerState::InitializeChampionData()
{
	if (!HasAuthority() || bChampionDataInitialized)
	{
		return;
	}

	const AXersuoGameMode* GameMode = GetWorld()->GetAuthGameMode<AXersuoGameMode>();
	const FChampionDataRow* ChampionData = GameMode
		? GameMode->FindChampionStats(SelectedChampionId)
		: nullptr;

	if (!ChampionData)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot initialize champion stats for %s: champion row '%s' is unavailable."),
			*GetName(), *SelectedChampionId.ToString());
		return;
	}

	bChampionDataInitialized = true;
	NormalAttackConfig = ChampionData->NormalAttackConfig;
	const float StartingHealth = ChampionData->MaxHealth.BaseValue;
	HealthAttributeSet->SetMaxHealth(StartingHealth);
	HealthAttributeSet->SetHealth(StartingHealth);
	BaseStatsAttributeSet->SetAttackDamage(ChampionData->AttackDamage.BaseValue);
	ForceNetUpdate();
}

void AXersuoPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AXersuoPlayerState, NormalAttackConfig);
}

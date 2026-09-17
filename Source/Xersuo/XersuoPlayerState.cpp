// Copyright Epic Games, Inc. All Rights Reserved.

#include "XersuoPlayerState.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AttributeSets/HealthAttributeSet.h"
#include "Champion/ChampionDataRow.h"
#include "Engine/World.h"
#include "XersuoGameMode.h"

AXersuoPlayerState::AXersuoPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	HealthAttributeSet = CreateDefaultSubobject<UHealthAttributeSet>(TEXT("HealthAttributeSet"));
}

UAbilitySystemComponent* AXersuoPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

void AXersuoPlayerState::InitializeChampionStats()
{
	if (!HasAuthority() || bChampionHealthInitialized)
	{
		return;
	}

	const AXersuoGameMode* GameMode = GetWorld()->GetAuthGameMode<AXersuoGameMode>();
	const FChampionDataRow* ChampionData = GameMode
		? GameMode->FindChampionStats(SelectedChampionId)
		: nullptr;

	if (!ChampionData)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot initialize health for %s: champion row '%s' is unavailable."),
			*GetName(), *SelectedChampionId.ToString());
		return;
	}

	const float StartingHealth = ChampionData->MaxHealth.BaseValue;
	if (!FMath::IsFinite(StartingHealth) || StartingHealth <= 0.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot initialize champion '%s': MaxHealth.BaseValue must be finite and positive."),
			*SelectedChampionId.ToString());
		return;
	}

	bChampionHealthInitialized = true;
	HealthAttributeSet->SetMaxHealth(StartingHealth);
	HealthAttributeSet->SetHealth(StartingHealth);
	ForceNetUpdate();
}

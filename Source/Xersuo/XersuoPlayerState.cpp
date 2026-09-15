// Copyright Epic Games, Inc. All Rights Reserved.

#include "XersuoPlayerState.h"
#include "AbilitySystemComponent.h"

AXersuoPlayerState::AXersuoPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
}

UAbilitySystemComponent* AXersuoPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

// Copyright Epic Games, Inc. All Rights Reserved.

#include "XersuoPlayerState.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AttributeSets/HealthAttributeSet.h"

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

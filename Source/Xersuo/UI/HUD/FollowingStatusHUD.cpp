// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/FollowingStatusHUD.h"
#include "AbilitySystemComponent.h"
#include "UI/HUD/AttributeBar.h"

void UFollowingStatusHUD::SetAbilitySystemComponent(UAbilitySystemComponent* InAbilitySystemComponent)
{
	if (!InAbilitySystemComponent)
	{
		return;
	}

	HealthBar->SetAbilitySystemComponent(InAbilitySystemComponent);
	// EnergyBar->SetAbilitySystemComponent(InAbilitySystemComponent);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/FollowingStatusHUD.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AttributeSets/HealthAttributeSet.h"
#include "Components/ProgressBar.h"

void UFollowingStatusHUD::SetAbilitySystemComponent(UAbilitySystemComponent* InAbilitySystemComponent)
{
	UnbindHealthDelegates();
	AbilitySystemComponent = InAbilitySystemComponent;

	if (InAbilitySystemComponent)
	{
		HealthChangedHandle = InAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			UHealthAttributeSet::GetHealthAttribute()).AddUObject(this, &ThisClass::OnHealthAttributeChanged);
		MaxHealthChangedHandle = InAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			UHealthAttributeSet::GetMaxHealthAttribute()).AddUObject(this, &ThisClass::OnHealthAttributeChanged);
	}

	RefreshHealthBar();
}

void UFollowingStatusHUD::NativeConstruct()
{
	Super::NativeConstruct();
	// Reconnect if the same widget is removed and subsequently displayed again.
	SetAbilitySystemComponent(AbilitySystemComponent.Get());
}

void UFollowingStatusHUD::NativeDestruct()
{
	UnbindHealthDelegates();
	Super::NativeDestruct();
}

void UFollowingStatusHUD::UnbindHealthDelegates()
{
	if (UAbilitySystemComponent* ASC = AbilitySystemComponent.Get())
	{
		ASC->GetGameplayAttributeValueChangeDelegate(UHealthAttributeSet::GetHealthAttribute()).Remove(HealthChangedHandle);
		ASC->GetGameplayAttributeValueChangeDelegate(UHealthAttributeSet::GetMaxHealthAttribute()).Remove(MaxHealthChangedHandle);
	}

	HealthChangedHandle.Reset();
	MaxHealthChangedHandle.Reset();
}

void UFollowingStatusHUD::OnHealthAttributeChanged(const FOnAttributeChangeData& Data) const
{
	RefreshHealthBar();
}

void UFollowingStatusHUD::RefreshHealthBar() const
{
	if (!HealthBar)
	{
		return;
	}

	const UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	if (!ASC)
	{
		return;
	}
	
	if (const UHealthAttributeSet* Attributes = ASC->GetSet<UHealthAttributeSet>())
	{
		const float Health = Attributes->GetHealth();
		const float MaxHealth = Attributes->GetMaxHealth();
		const float Percent = FMath::Clamp(Health / MaxHealth, 0.f, 1.f);
		HealthBar->SetPercent(Percent);
	}
}


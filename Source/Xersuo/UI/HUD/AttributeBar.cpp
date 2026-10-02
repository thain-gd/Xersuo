#include "UI/HUD/AttributeBar.h"

#include "AbilitySystemComponent.h"
#include "Components/ProgressBar.h"

void UAttributeBar::SetAbilitySystemComponent(UAbilitySystemComponent* InAbilitySystemComponent)
{
	AbilitySystemComponent = InAbilitySystemComponent;

	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	if (ASC && ASC->HasAttributeSetForAttribute(CurrentAttribute) && ASC->HasAttributeSetForAttribute(MaxAttribute))
	{
		CurrentChangedHandle = ASC->GetGameplayAttributeValueChangeDelegate(CurrentAttribute).AddUObject(this, &ThisClass::OnAttributeChanged);
		MaxChangedHandle = ASC->GetGameplayAttributeValueChangeDelegate(MaxAttribute).AddUObject(this, &ThisClass::OnAttributeChanged);
	}

	RefreshBar();
}

void UAttributeBar::NativeDestruct()
{
	if (UAbilitySystemComponent* ASC = AbilitySystemComponent.Get())
	{
		if (CurrentChangedHandle.IsValid())
		{
			ASC->GetGameplayAttributeValueChangeDelegate(CurrentAttribute).Remove(CurrentChangedHandle);
		}

		if (MaxChangedHandle.IsValid())
		{
			ASC->GetGameplayAttributeValueChangeDelegate(MaxAttribute).Remove(MaxChangedHandle);
		}
	}

	CurrentChangedHandle.Reset();
	MaxChangedHandle.Reset();

	Super::NativeDestruct();
}

void UAttributeBar::OnAttributeChanged(const FOnAttributeChangeData& Data) const
{
	RefreshBar();
}

void UAttributeBar::RefreshBar() const
{
	if (!Bar)
	{
		return;
	}

	const UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	if (!ASC || !ASC->HasAttributeSetForAttribute(CurrentAttribute) || !ASC->HasAttributeSetForAttribute(MaxAttribute))
	{
		Bar->SetPercent(0.f);
		return;
	}

	const float CurrentValue = ASC->GetNumericAttribute(CurrentAttribute);
	const float MaxValue = ASC->GetNumericAttribute(MaxAttribute);
	const float Percent = MaxValue > 0.f ? FMath::Clamp(CurrentValue / MaxValue, 0.f, 1.f) : 0.f;
	Bar->SetPercent(Percent);
}

#include "UI/HUD/PlayerMainHUD.h"

#include "UI/HUD/AttributeBar.h"

void UPlayerMainHUD::SetAbilitySystemComponent(UAbilitySystemComponent* InAbilitySystemComponent)
{
	if (!InAbilitySystemComponent || bAttributesInitialized)
	{
		return;
	}

	HealthBar->SetAbilitySystemComponent(InAbilitySystemComponent);
	bAttributesInitialized = true;
}

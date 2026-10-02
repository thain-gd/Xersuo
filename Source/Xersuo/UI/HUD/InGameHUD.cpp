#include "UI/HUD/InGameHUD.h"

#include "UI/HUD/PlayerMainHUD.h"

void UInGameHUD::SetAbilitySystemComponent(UAbilitySystemComponent* InAbilitySystemComponent)
{
	PlayerMainHUD->SetAbilitySystemComponent(InAbilitySystemComponent);
}

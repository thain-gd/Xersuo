#include "UI/HUD/XersuoHUD.h"

#include "GameFramework/PlayerController.h"
#include "UI/HUD/InGameHUD.h"
#include "AbilitySystemComponent.h"
#include "XersuoPlayerState.h"

void AXersuoHUD::SetAbilitySystemComponent(UAbilitySystemComponent* InAbilitySystemComponent)
{
	if (InGameHUD)
	{
		InGameHUD->SetAbilitySystemComponent(InAbilitySystemComponent);
	}
}

void AXersuoHUD::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = GetOwningPlayerController();
	if (!PlayerController || !PlayerController->IsLocalController() || !InGameHUDClass)
	{
		return;
	}

	InGameHUD = CreateWidget<UInGameHUD>(PlayerController, InGameHUDClass);
	if (InGameHUD)
	{
		InGameHUD->AddToPlayerScreen();
	}
}

void AXersuoHUD::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (InGameHUD)
	{
		InGameHUD->RemoveFromParent();
		InGameHUD = nullptr;
	}

	Super::EndPlay(EndPlayReason);
}

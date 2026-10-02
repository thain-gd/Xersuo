#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "XersuoHUD.generated.h"

class UInGameHUD;
class UAbilitySystemComponent;

UCLASS()
class XERSUO_API AXersuoHUD : public AHUD
{
	GENERATED_BODY()

public:
	void SetAbilitySystemComponent(UAbilitySystemComponent* InAbilitySystemComponent);

	UInGameHUD* GetInGameHUD() const { return InGameHUD.Get(); }

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	TSubclassOf<UInGameHUD> InGameHUDClass;

private:
	UPROPERTY(Transient)
	TObjectPtr<UInGameHUD> InGameHUD;
};

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameHUD.generated.h"

class UPerformanceHUD;
class UPlayerMainHUD;
class UAbilitySystemComponent;

UCLASS(Abstract)
class XERSUO_API UInGameHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetAbilitySystemComponent(UAbilitySystemComponent* InAbilitySystemComponent);

	UPlayerMainHUD* GetPlayerMainHUD() const { return PlayerMainHUD.Get(); }

protected:
	UPROPERTY(BlueprintReadOnly, Category = "HUD", meta = (BindWidget))
	TObjectPtr<UPerformanceHUD> Performance;

	UPROPERTY(BlueprintReadOnly, Category = "HUD", meta = (BindWidget))
	TObjectPtr<UPlayerMainHUD> PlayerMainHUD;
};

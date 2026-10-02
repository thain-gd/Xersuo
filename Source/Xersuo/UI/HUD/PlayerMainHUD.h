#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerMainHUD.generated.h"

class UAttributeBar;
class UImage;
class UProgressBar;
class UAbilitySystemComponent;

UCLASS(Abstract)
class XERSUO_API UPlayerMainHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetAbilitySystemComponent(UAbilitySystemComponent* InAbilitySystemComponent);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "HUD", meta = (BindWidget))
	TObjectPtr<UImage> Avatar;

	UPROPERTY(BlueprintReadOnly, Category = "HUD", meta = (BindWidget))
	TObjectPtr<UProgressBar> ExperienceBar;

	UPROPERTY(BlueprintReadOnly, Category = "HUD", meta = (BindWidget))
	TObjectPtr<UAttributeBar> HealthBar;

private:
	bool bAttributesInitialized = false;
};

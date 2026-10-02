#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AbilitySlot.generated.h"

class UButton;
class UImage;
class UTextBlock;

UCLASS(Abstract)
class XERSUO_API UAbilitySlot : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Ability", meta = (BindWidget))
	TObjectPtr<UButton> ActivateButton;

	UPROPERTY(BlueprintReadOnly, Category = "Ability", meta = (BindWidget))
	TObjectPtr<UImage> AbilityIcon;

	UPROPERTY(BlueprintReadOnly, Category = "Ability", meta = (BindWidget))
	TObjectPtr<UImage> CooldownShade;

	UPROPERTY(BlueprintReadOnly, Category = "Ability", meta = (BindWidget))
	TObjectPtr<UTextBlock> CooldownText;

	UPROPERTY(BlueprintReadOnly, Category = "Ability", meta = (BindWidget))
	TObjectPtr<UTextBlock> KeyLabelText;
};

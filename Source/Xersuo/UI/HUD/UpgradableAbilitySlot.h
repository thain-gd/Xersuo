#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UpgradableAbilitySlot.generated.h"

class UAbilitySlot;
class UButton;
class UHorizontalBox;

UCLASS(Abstract)
class XERSUO_API UUpgradableAbilitySlot : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Ability", meta = (BindWidget))
	TObjectPtr<UAbilitySlot> CoreAbilitySlot;
	
	UPROPERTY(BlueprintReadOnly, Category = "Ability", meta = (BindWidget))
	TObjectPtr<UHorizontalBox> RankIndicatorBar;

	UPROPERTY(BlueprintReadOnly, Category = "Ability", meta = (BindWidget))
	TObjectPtr<UButton> RankUpButton;
};

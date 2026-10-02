// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FollowingStatusHUD.generated.h"

class UBorder;
class UAttributeBar;
class UTextBlock;
class UAbilitySystemComponent;

// A widget for the HUD status (health, mana/energy, level) that follows champions around
UCLASS()
class XERSUO_API UFollowingStatusHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	/** Bind both bars to the first non-null ability system component. */
	UFUNCTION(BlueprintCallable, Category = "Status")
	void SetAbilitySystemComponent(UAbilitySystemComponent* InAbilitySystemComponent);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Status", meta = (BindWidget))
	TObjectPtr<UBorder> LevelBorder;

	UPROPERTY(BlueprintReadOnly, Category = "Status", meta = (BindWidget))
	TObjectPtr<UTextBlock> LevelText;

	UPROPERTY(BlueprintReadOnly, Category = "Status", meta = (BindWidget))
	TObjectPtr<UAttributeBar> HealthBar;

	UPROPERTY(BlueprintReadOnly, Category = "Status", meta = (BindWidget))
	TObjectPtr<UAttributeBar> EnergyBar;
};

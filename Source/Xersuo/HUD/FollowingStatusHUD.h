// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FollowingStatusHUD.generated.h"

class UBorder;
class UProgressBar;
class UTextBlock;
class UAbilitySystemComponent;
struct FOnAttributeChangeData;

// A widget for the HUD status (health, mana/energy, level) that follows champions around
UCLASS()
class XERSUO_API UFollowingStatusHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	/** Display this champion's attributes; passing null disconnects and clears the bar. */
	UFUNCTION(BlueprintCallable, Category = "Status")
	void SetAbilitySystemComponent(UAbilitySystemComponent* InAbilitySystemComponent);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UPROPERTY(BlueprintReadOnly, Category = "Status", meta = (BindWidget))
	TObjectPtr<UBorder> LevelBorder;

	UPROPERTY(BlueprintReadOnly, Category = "Status", meta = (BindWidget))
	TObjectPtr<UTextBlock> LevelText;

	UPROPERTY(BlueprintReadOnly, Category = "Status", meta = (BindWidget))
	TObjectPtr<UProgressBar> HealthBar;

	UPROPERTY(BlueprintReadOnly, Category = "Status", meta = (BindWidget))
	TObjectPtr<UProgressBar> EnergyBar;

private:
	void UnbindHealthDelegates();
	void OnHealthAttributeChanged(const FOnAttributeChangeData& Data) const;
	void RefreshHealthBar() const;

	TWeakObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	FDelegateHandle HealthChangedHandle;
	FDelegateHandle MaxHealthChangedHandle;
};

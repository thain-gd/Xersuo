#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "Blueprint/UserWidget.h"
#include "AttributeBar.generated.h"

class UAbilitySystemComponent;
class UProgressBar;
class UTextBlock;
struct FOnAttributeChangeData;

UCLASS(Abstract)
class XERSUO_API UAttributeBar : public UUserWidget
{
	GENERATED_BODY()

public:
	/** Call once after construction to bind this bar to its attribute source. */
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void SetAbilitySystemComponent(UAbilitySystemComponent* InAbilitySystemComponent);

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeDestruct() override;

	UPROPERTY(EditAnywhere, Category = "Attributes")
	FGameplayAttribute CurrentAttribute;

	UPROPERTY(EditAnywhere, Category = "Attributes")
	FGameplayAttribute MaxAttribute;
	
	UPROPERTY(EditAnywhere, Category = "Attributes")
	FLinearColor FillColor = FColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
	bool bShowValueText = false;

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> ValueText;

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", meta = (BindWidget))
	TObjectPtr<UProgressBar> Bar;

private:
	void OnAttributeChanged(const FOnAttributeChangeData& Data) const;
	void RefreshBar() const;

	TWeakObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	FDelegateHandle CurrentChangedHandle;
	FDelegateHandle MaxChangedHandle;
};

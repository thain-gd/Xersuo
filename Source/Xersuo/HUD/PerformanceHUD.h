#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PerformanceHUD.generated.h"

class UTextBlock;

UCLASS()
class XERSUO_API UPerformanceHUD : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(BlueprintReadOnly, Category = "Performance", meta = (BindWidget))
	TObjectPtr<UTextBlock> FPSText;

	UPROPERTY(BlueprintReadOnly, Category = "Performance", meta = (BindWidget))
	TObjectPtr<UTextBlock> PingText;

private:
	int32 FrameCount = 0;
	float ElapsedTime = 0.0f;
};

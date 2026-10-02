#include "UI/HUD/PerformanceHUD.h"

#include "Components/TextBlock.h"
#include "GameFramework/PlayerState.h"

void UPerformanceHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	++FrameCount;
	ElapsedTime += InDeltaTime;

	if (ElapsedTime < 0.5f)
	{
		return;
	}

	if (FPSText)
	{
		const int32 FPS = FMath::RoundToInt(FrameCount / ElapsedTime);
		FPSText->SetText(FText::Format(NSLOCTEXT("PerformanceHUD", "FPS", "FPS: {0}"), FText::AsNumber(FPS)));
	}

	if (PingText)
	{
		if (const APlayerState* PlayerState = GetOwningPlayerState<APlayerState>())
		{
			const int32 Ping = FMath::RoundToInt(PlayerState->GetPingInMilliseconds());
			PingText->SetText(FText::Format(NSLOCTEXT("PerformanceHUD", "Ping", "Ping: {0} ms"), FText::AsNumber(Ping)));
		}
		else
		{
			PingText->SetText(NSLOCTEXT("PerformanceHUD", "PingUnavailable", "Ping: -- ms"));
		}
	}

	FrameCount = 0;
	ElapsedTime = 0.0f;
}

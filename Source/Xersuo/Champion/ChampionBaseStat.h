#pragma once

#include "CoreMinimal.h"

#include "ChampionBaseStat.generated.h"

USTRUCT(BlueprintType)
struct FChampionBaseStat
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.0))
	float BaseValue = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.0))
	float GrowthPerLevel = 0.f;
};

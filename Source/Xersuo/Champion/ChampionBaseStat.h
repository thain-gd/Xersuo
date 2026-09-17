#pragma once

#include "CoreMinimal.h"

#include "ChampionBaseStat.generated.h"

USTRUCT(BlueprintType)
struct FChampionBaseStat
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float BaseValue = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float GrowthPerLevel = 0.f;
};

#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ChampionBaseStat.h"
#include "XersuoCharacter.h"

#include "ChampionDataRow.generated.h"

USTRUCT(BlueprintType)
struct FChampionDataRow : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText DisplayName;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AXersuoCharacter> CharacterClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FChampionBaseStat MaxHealth;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FChampionBaseStat AttackDamage;
};


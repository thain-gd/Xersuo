#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ChampionBaseStat.h"
#include "XersuoCharacter.h"

#include "ChampionDataRow.generated.h"

class UNormalAttackConfig;

USTRUCT(BlueprintType)
struct FChampionDataRow : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText DisplayName;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AXersuoCharacter> CharacterClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FChampionBaseStat MaxHealth;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FChampionBaseStat AttackDamage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UNormalAttackConfig> NormalAttackConfig = nullptr;
};


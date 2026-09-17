// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "XersuoGameMode.generated.h"

struct FChampionDataRow;
/**
 *  Simple Game Mode for a top-down perspective game
 *  Sets the default gameplay framework classes
 *  Check the Blueprint derived class for the set values
 */
UCLASS(abstract)
class AXersuoGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	/** Constructor */
	AXersuoGameMode();
	
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;
	
	const FChampionDataRow* FindChampionStats(FName ChampionId) const;
	
	UPROPERTY(EditDefaultsOnly, Category = "Champions")
	TObjectPtr<UDataTable> ChampionStatsTable;
};




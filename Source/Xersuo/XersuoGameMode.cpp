// Copyright Epic Games, Inc. All Rights Reserved.

#include "XersuoGameMode.h"

#include "XersuoPlayerState.h"
#include "Champion/ChampionDataRow.h"

AXersuoGameMode::AXersuoGameMode()
{
	PlayerStateClass = AXersuoPlayerState::StaticClass();
}

UClass* AXersuoGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	if (const AXersuoPlayerState* PlayerState = Cast<AXersuoPlayerState>(InController->GetPlayerState<AXersuoPlayerState>()))
	{
		if (const FChampionDataRow* ChampionRow = FindChampionStats(PlayerState->GetSelectedChampionId()))
		{
			checkf(ChampionRow->CharacterClass, TEXT("%s's character class is not valid"), *ChampionRow->DisplayName.ToString());
			return ChampionRow->CharacterClass.Get();
		}
	}
	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

const FChampionDataRow* AXersuoGameMode::FindChampionStats(FName ChampionId) const
{
	if (!ChampionStatsTable)
	{
		return nullptr;
	}
	
	return ChampionStatsTable->FindRow<FChampionDataRow>(ChampionId, TEXT("FindChampionStats"));
}

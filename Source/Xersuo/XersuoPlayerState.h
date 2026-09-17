// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "XersuoPlayerState.generated.h"

class UAbilitySystemComponent;
class UHealthAttributeSet;

/** Owns the champion's ability system so it persists across pawn respawns. */
UCLASS()
class XERSUO_API AXersuoPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AXersuoPlayerState();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	FName GetSelectedChampionId() const { return SelectedChampionId; }

	/** Initializes level-one health on the server after the ASC has an avatar. */
	void InitializeChampionStats();
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UHealthAttributeSet> HealthAttributeSet;
	
	// TODO: Replace this with the proper selected champion id from the lobby
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Champion", meta = (AllowPrivateAccess = "true"))
	FName SelectedChampionId = TEXT("Phase");

	bool bChampionHealthInitialized = false;
};

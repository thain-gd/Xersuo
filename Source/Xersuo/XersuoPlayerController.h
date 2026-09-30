// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
//#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "XersuoPlayerController.generated.h"

class UNiagaraSystem;
class UInputMappingContext;
class UInputAction;
class UPathFollowingComponent;
class UGameplayAbility;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

/**
 *  Player controller for a top-down perspective game.
 *  Implements point and click based controls
 */
UCLASS(abstract)
class AXersuoPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:

	/** Component used for moving along a NavMesh path. */
	UPROPERTY(VisibleDefaultsOnly, Category = AI)
	TObjectPtr<UPathFollowingComponent> PathFollowingComponent;

	/** Time Threshold to know if it was a short press */
	UPROPERTY(EditAnywhere, Category="Input")
	float ShortPressThreshold;

	/** FX Class that we will spawn when clicking */
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UNiagaraSystem> FXCursor;

	/** MappingContext */
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;
	
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> SetDestinationClickAction;

	/** Shared normal attack implementation, granted on the server when first needed. */
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<UGameplayAbility> NormalAttackAbility;

	/** Saved location of the character movement destination */
	FVector CachedDestination;

	/** Time that the click input has been pressed */
	float FollowTime = 0.0f;

public:

	/** Constructor */
	AXersuoPlayerController();
	
	virtual void Tick(float DeltaTime) override;
	virtual void PlayerTick(float DeltaTime) override;
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

protected:

	/** Initialize input bindings */
	virtual void SetupInputComponent() override;
	
	void OnInputStarted();
	void OnSetDestinationTriggered();
	void OnSetDestinationReleased();

	/** Helper function to get the move destination */
	void UpdateCachedDestination();
	
private:
	UFUNCTION(Server, Reliable)
	void ServerCancelAttack();

	UFUNCTION(Server, Reliable)
	void ServerProcessNormalAttack(AActor* Target);

	UFUNCTION(Server, Reliable)
	void ServerReadyToAttack();
	
	UFUNCTION(Client, Reliable)
	void ClientStartAttackMovement(AActor* Target, APawn* OrderPawn, float AttackRange);

	UFUNCTION(Client, Reliable)
	void ClientStopAttackMovement();

	void CheckForNormalAttack();
	void UpdateClientAttackApproach();
	bool MoveIntoAttackRange(AActor* Target, float AttackRange);
	void CancelAttack();
	void StopAttackMovement();

	UPROPERTY(Replicated)
	bool bMovingToAttack = false;
	
	UPROPERTY(Replicated)
	bool bIsAttacking = false;
	
	TWeakObjectPtr<AActor> CurrentAttackTarget;
	float ClientAttackRange = 0.f;

	void ProcessCursorTrace();
	void SetHoveredActorHighlight(bool bEnabled) const;
	
	TWeakObjectPtr<AActor> CurrentHoveredActor;
};



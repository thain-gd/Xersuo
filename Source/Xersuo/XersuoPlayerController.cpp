// Copyright Epic Games, Inc. All Rights Reserved.

#include "XersuoPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "XersuoCharacter.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "Xersuo.h"
#include "Collision/CollisionChannels.h"
#include "Visual/HighlightComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/Abilities/NormalAttack.h"
#include "Champion/NormalAttackConfig.h"
#include "XersuoPlayerState.h"
#include "NavigationSystem.h"
#include "AbilitySystem/GameplayTags/XersuoGameplayTags.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

AXersuoPlayerController::AXersuoPlayerController()
{
	// create the path following comp
	PathFollowingComponent = CreateDefaultSubobject<UPathFollowingComponent>(TEXT("Path Following Component"));

	// configure the controller
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;
	NormalAttackAbility = UNormalAttack::StaticClass();
}

void AXersuoPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	
	if (IsLocalController())
	{
		ProcessCursorTrace();
	}
	
	if (bIsAttacking)
	{
		if (IsLocalController() && bMovingToAttack)
		{
			UpdateClientAttackApproach();
		}
		else if (HasAuthority())
		{
			CheckForNormalAttack();
		}
	}
}

void AXersuoPlayerController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AXersuoPlayerController, bMovingToAttack);
	DOREPLIFETIME(AXersuoPlayerController, bIsAttacking);
}

void AXersuoPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// Only set up input on local player controllers
	if (IsLocalPlayerController())
	{
		// Add Input Mapping Context
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}

		// Set up action bindings
		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
		{
			// Setup mouse input events
			EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Started, this, &AXersuoPlayerController::OnInputStarted);
			EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Triggered, this, &AXersuoPlayerController::OnSetDestinationTriggered);
			EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Completed, this, &AXersuoPlayerController::OnSetDestinationReleased);
			EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Canceled, this, &AXersuoPlayerController::OnSetDestinationReleased);
		}
		else
		{
			UE_LOG(LogXersuo, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
		}
	}
}

void AXersuoPlayerController::OnInputStarted()
{
	FollowTime = 0.f;
	StopMovement();
	
	ProcessCursorTrace();
	if (AActor* Target = CurrentHoveredActor.Get())
	{
		ServerProcessNormalAttack(Target);
	}
	else
	{
		if (bIsAttacking)
		{
			ServerCancelAttack();
		}
		
		// Update the move destination to wherever the cursor is pointing at
    	UpdateCachedDestination();
	}
}

void AXersuoPlayerController::OnSetDestinationTriggered()
{
	if (bIsAttacking)
	{
		return;
	}

	// We flag that the input is being pressed
	FollowTime += GetWorld()->GetDeltaSeconds();
	
	// Update the move destination to wherever the cursor is pointing at
	UpdateCachedDestination();
	
	// Move towards mouse pointer
	APawn* ControlledPawn = GetPawn();
	if (ControlledPawn != nullptr)
	{
		FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
		ControlledPawn->AddMovementInput(WorldDirection, 1.0, false);
	}
}

void AXersuoPlayerController::OnSetDestinationReleased()
{
	// If it was a short press
	if (!bIsAttacking && FollowTime <= ShortPressThreshold)
	{
		// Connects the path follower to the currently possessed CMC
		PathFollowingComponent->Initialize();

		// We move there and spawn some particles
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, CachedDestination);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, CachedDestination, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
	}
}

void AXersuoPlayerController::ServerCancelAttack_Implementation()
{
	bIsAttacking = false;
	CancelAttack();

	const AXersuoPlayerState* ChampionState = GetPlayerState<AXersuoPlayerState>();
	UAbilitySystemComponent* ASC = ChampionState ? ChampionState->GetAbilitySystemComponent() : nullptr;
	if (ASC && NormalAttackAbility)
	{
		const FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromClass(NormalAttackAbility);
		if (Spec && Spec->IsActive())
		{
			ASC->CancelAbilityHandle(Spec->Handle);
		}
	}
}

void AXersuoPlayerController::StopAttackMovement()
{
	StopMovement();
	if (AXersuoCharacter* XersuoChar = Cast<AXersuoCharacter>(GetPawn()))
	{
		XersuoChar->ConsumeMovementInputVector();
		XersuoChar->GetCharacterMovement()->StopMovementImmediately();
	}
}

void AXersuoPlayerController::CancelAttack()
{
	if (HasAuthority() && !IsLocalController())
	{
		ClientStopAttackMovement();
	}
	
	bIsAttacking = false;
	bMovingToAttack = false;
	CurrentAttackTarget.Reset();
}

void AXersuoPlayerController::ServerProcessNormalAttack_Implementation(AActor* Target)
{
	if (CurrentAttackTarget == Target || !IsValid(Target))
	{
		return;
	}
	
	CancelAttack();
	bIsAttacking = true;
	CurrentAttackTarget = Target;
	CheckForNormalAttack();
}

void AXersuoPlayerController::ClientStartAttackMovement_Implementation(
	AActor* Target, APawn* OrderPawn, float AttackRange)
{
	CurrentAttackTarget = Target;
	ClientAttackRange = AttackRange;
	MoveIntoAttackRange(Target, AttackRange);
}

void AXersuoPlayerController::ClientStopAttackMovement_Implementation()
{
	StopAttackMovement();
}

void AXersuoPlayerController::UpdateClientAttackApproach()
{
	APawn* ControlledPawn = GetPawn();
	AActor* Target = CurrentAttackTarget.Get();
	if (!ControlledPawn || !IsValid(Target))
	{
		CancelAttack();
		return;
	}

	// Stop slightly inside range to give the server room for movement/position differences.
	const float StopRange = FMath::Max(0.f, ClientAttackRange - 5.f);
	if (FVector::DistSquared2D(ControlledPawn->GetActorLocation(), Target->GetActorLocation()) <= FMath::Square(StopRange))
	{
		StopAttackMovement();
		ServerReadyToAttack();
		return;
	}

	// Find a new path toward the target again if the current path is done and still outside of the attack range
	if (PathFollowingComponent->GetStatus() == EPathFollowingStatus::Idle)
	{
		MoveIntoAttackRange(Target, ClientAttackRange);
	}
}

void AXersuoPlayerController::CheckForNormalAttack()
{
	APawn* ControlledPawn = GetPawn();
	AActor* Target = CurrentAttackTarget.Get();
	const AXersuoPlayerState* ChampionState = GetPlayerState<AXersuoPlayerState>();
	const UNormalAttackConfig* Config = ChampionState ? ChampionState->GetNormalAttackConfig() : nullptr;
	UAbilitySystemComponent* ASC = ChampionState ? ChampionState->GetAbilitySystemComponent() : nullptr;
	if (!ControlledPawn || !IsValid(Target) || !Config || !ASC || !NormalAttackAbility)
	{
		CancelAttack();
		return;
	}
	if (!ASC->AbilityActorInfo.IsValid() || ASC->AbilityActorInfo->AvatarActor.Get() != ControlledPawn)
	{
		CancelAttack();
		return;
	}

	// Ground-plane, center-to-center range, independent of capsule sizes.
	const float DistanceSquared = FVector::DistSquared2D(ControlledPawn->GetActorLocation(), Target->GetActorLocation());
	if (DistanceSquared > FMath::Square(Config->BaseAttackRange))
	{
		if (!bMovingToAttack)
		{
			bMovingToAttack = true;
			ClientStartAttackMovement(Target, ControlledPawn, Config->BaseAttackRange);
		}
		return;
	}

	// An attack order must execute on the server, never forward activation to an untrusted client.
	const UGameplayAbility* AbilityDefaults = NormalAttackAbility->GetDefaultObject<UGameplayAbility>();
	if (AbilityDefaults->GetNetExecutionPolicy() != EGameplayAbilityNetExecutionPolicy::ServerOnly
		&& AbilityDefaults->GetNetExecutionPolicy() != EGameplayAbilityNetExecutionPolicy::ServerInitiated)
	{
		UE_LOG(LogXersuo, Warning, TEXT("NormalAttackAbility must use ServerOnly or ServerInitiated execution."));
		CancelAttack();
		return;
	}

	const FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromClass(NormalAttackAbility);
	if (Spec && Spec->IsActive())
	{
		return;
	}
	const FGameplayAbilitySpecHandle Handle = Spec ? Spec->Handle
		: ASC->GiveAbility(FGameplayAbilitySpec(NormalAttackAbility, 1, INDEX_NONE));
	FGameplayEventData AttackEvent;
	AttackEvent.Instigator = ControlledPawn;
	AttackEvent.Target = Target;
	if (!ASC->TriggerAbilityFromGameplayEvent(Handle, ASC->AbilityActorInfo.Get(), XersuoGameplayTags::Event_Attack_Activate, &AttackEvent, *ASC))
	{
		CancelAttack();
	}
}

bool AXersuoPlayerController::MoveIntoAttackRange(AActor* Target, float AttackRange)
{
	// Path following generates input on the machine that locally controls this player pawn.
	if (!IsLocalController() || !GetPawn() || !IsValid(Target))
	{
		return false;
	}
	UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (!NavSystem)
	{
		return false;
	}
	PathFollowingComponent->Initialize();
	if (!PathFollowingComponent->IsPathFollowingAllowed())
	{
		return false;
	}
	const FVector Start = GetNavAgentLocation();
	const ANavigationData* NavData = NavSystem->GetNavDataForProps(GetNavAgentPropertiesRef(), Start);
	if (!NavData)
	{
		return false;
	}

	FPathFindingQuery Query(this, *NavData, Start, Target->GetActorLocation());
	FPathFindingResult Result = NavSystem->FindPathSync(Query);
	if (!Result.IsSuccessful() || !Result.Path.IsValid())
	{
		return false;
	}
	Result.Path->SetGoalActorObservation(*Target, 25.f);
	FAIMoveRequest Request(Target);
	// Stop slightly inside the range; exclude collision radii from the distance calculation.
	Request.SetAcceptanceRadius(FMath::Max(0.f, AttackRange - 5.f));
	Request.SetReachTestIncludesAgentRadius(false);
	Request.SetReachTestIncludesGoalRadius(false);
	StopMovement();
	return PathFollowingComponent->RequestMove(Request, Result.Path).IsValid();
}

void AXersuoPlayerController::UpdateCachedDestination()
{
	// If we hit a surface, cache the location
	FHitResult Hit;
	if (GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit))
	{
		CachedDestination = Hit.Location;
	}
}

void AXersuoPlayerController::ServerReadyToAttack_Implementation()
{
	bMovingToAttack = false;
	CheckForNormalAttack();
}

void AXersuoPlayerController::ProcessCursorTrace()
{
	FVector Origin;
	FVector Direction;
	if (!DeprojectMousePositionToWorld(Origin, Direction))
	{
		SetHoveredActorHighlight(false);
		CurrentHoveredActor.Reset();
		return;
	}
	
	FCollisionQueryParams Params;
	Params.bTraceComplex = false;
	if (APawn* OwnPawn = GetPawn())
	{
		Params.AddIgnoredActor(OwnPawn);
	}
	
	FHitResult Hit;
	if (GetWorld()->LineTraceSingleByChannel(Hit, Origin, Origin + Direction * HitResultTraceDistance, XersuoCollision::Targeting, Params))
	{
		AActor* HitActor = Hit.GetActor();
		if (CurrentHoveredActor == nullptr || CurrentHoveredActor != HitActor)
		{
			if (CurrentHoveredActor != nullptr)
			{
				SetHoveredActorHighlight(false);
			}
			
			CurrentHoveredActor = HitActor;
			SetHoveredActorHighlight(true);
		}
	}
	else if (CurrentHoveredActor != nullptr)
	{
		SetHoveredActorHighlight(false);
		CurrentHoveredActor = nullptr;
	}
}

void AXersuoPlayerController::SetHoveredActorHighlight(bool bEnabled) const
{
	if (AActor* HoveredActor = CurrentHoveredActor.Get())
	{
		if (UHighlightComponent* HighlightComponent = HoveredActor->GetComponentByClass<UHighlightComponent>())
		{
			HighlightComponent->SetHighlightEnabled(bEnabled);
		}
	}
}

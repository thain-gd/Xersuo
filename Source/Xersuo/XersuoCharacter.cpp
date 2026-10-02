// Copyright Epic Games, Inc. All Rights Reserved.

#include "XersuoCharacter.h"
#include "XersuoPlayerState.h"
#include "AbilitySystemComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "Visual/HighlightComponent.h"
#include "Components/WidgetComponent.h"
#include "UI/HUD/FollowingStatusHUD.h"
#include "UI/HUD/XersuoHUD.h"

AXersuoCharacter::AXersuoCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	GetCharacterMovement()->GetNavMovementProperties()->bUseAccelerationForPaths = true;

	// Create the camera boom component
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));

	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false;

	// Create the camera component
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));

	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false;
	
	HighlightComponent = CreateDefaultSubobject<UHighlightComponent>(TEXT("HighlightComponent"));

	FollowingStatusWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("FollowingStatusWidget"));
	FollowingStatusWidget->SetupAttachment(GetRootComponent());
	FollowingStatusWidget->SetWidgetSpace(EWidgetSpace::Screen);
	
	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

UAbilitySystemComponent* AXersuoCharacter::GetAbilitySystemComponent() const
{
	const AXersuoPlayerState* XersuoPlayerState = GetPlayerState<AXersuoPlayerState>();
	return XersuoPlayerState ? XersuoPlayerState->GetAbilitySystemComponent() : nullptr;
}

void AXersuoCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	InitializeAbilitySystem();

	if (AXersuoPlayerState* XersuoPlayerState = GetPlayerState<AXersuoPlayerState>())
	{
		XersuoPlayerState->InitializeChampionData();
	}
}

void AXersuoCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	InitializeAbilitySystem();
}

void AXersuoCharacter::OnRep_Controller()
{
	Super::OnRep_Controller();
	InitializePlayerHUD();
}

void AXersuoCharacter::InitializeAbilitySystem()
{
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		ASC->InitAbilityActorInfo(GetPlayerState<AXersuoPlayerState>(), this);
		InitializePlayerHUD();
	}

	if (HasActorBegunPlay())
	{
		InitializeFollowingStatusHUD();
	}
}

void AXersuoCharacter::InitializePlayerHUD() const
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (!ASC)
	{
		return;
	}

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController && PlayerController->IsLocalController())
	{
		if (AXersuoHUD* HUD = PlayerController->GetHUD<AXersuoHUD>())
		{
			HUD->SetAbilitySystemComponent(ASC);
		}
	}
}

void AXersuoCharacter::InitializeFollowingStatusHUD() const
{
	if (GetNetMode() == NM_DedicatedServer)
	{
		return;
	}

	FollowingStatusWidget->InitWidget();
	if (UFollowingStatusHUD* StatusHUD = Cast<UFollowingStatusHUD>(FollowingStatusWidget->GetUserWidgetObject()))
	{
		StatusHUD->SetAbilitySystemComponent(GetAbilitySystemComponent());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("FollowingStatusHUD Init Failed"));
	}
}

void AXersuoCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitializeFollowingStatusHUD();
	
	// Allow sockets to be updated so that spawning objects using sockets work properly, e.g., projectiles spawned from hands
	if (HasAuthority())
	{
		GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
	}
}

FVector AXersuoCharacter::GetProjectileTargetLocation() const
{
	return GetMesh()->GetSocketLocation(TEXT("ProjectileImpact"));
}

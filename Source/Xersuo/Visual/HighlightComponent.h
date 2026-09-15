// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HighlightComponent.generated.h"


// Used for highlighting hovered/selected champions/minions/turrets
UCLASS( ClassGroup=(Rendering), meta=(BlueprintSpawnableComponent) )
class XERSUO_API UHighlightComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHighlightComponent();
	
	void SetHighlightEnabled(bool bEnabled);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category="Highlight", meta=(ClampMin="1", ClampMax="255"))
	int32 StencilValue = 1;

private:
	UPROPERTY(Transient)
	TArray<TObjectPtr<UMeshComponent>> HighlightMeshes;
};

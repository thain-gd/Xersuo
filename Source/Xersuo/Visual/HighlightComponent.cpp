// Fill out your copyright notice in the Description page of Project Settings.


#include "Visual/HighlightComponent.h"

// Sets default values for this component's properties
UHighlightComponent::UHighlightComponent()
{
}

void UHighlightComponent::SetHighlightEnabled(bool bEnabled)
{
	for (UMeshComponent* Mesh : HighlightMeshes)
	{
		if (IsValid(Mesh))
		{
			Mesh->SetCustomDepthStencilValue(1);
			Mesh->SetRenderCustomDepth(bEnabled);
		}
	}
}


// Called when the game starts
void UHighlightComponent::BeginPlay()
{
	Super::BeginPlay();

	TInlineComponentArray<UMeshComponent*> Meshes;
	GetOwner()->GetComponents(Meshes);
	for (UMeshComponent* Mesh : Meshes)
	{
		HighlightMeshes.Add(Mesh);
	}
}

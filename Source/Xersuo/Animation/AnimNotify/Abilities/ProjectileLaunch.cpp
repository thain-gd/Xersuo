#include "ProjectileLaunch.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "AbilitySystem/GameplayTags/XersuoGameplayTags.h"
#include "Components/SkeletalMeshComponent.h"

void UProjectileLaunch::Notify(
	USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp || !MeshComp->GetOwner())
	{
		return;
	}

	FGameplayEventData Payload;
	Payload.EventTag = XersuoGameplayTags::Event_Attack_ProjectileLaunch;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(MeshComp->GetOwner(), Payload.EventTag, Payload);
}

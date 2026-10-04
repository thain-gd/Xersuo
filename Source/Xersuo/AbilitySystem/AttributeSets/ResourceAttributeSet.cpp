#include "AbilitySystem/AttributeSets/ResourceAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

UResourceAttributeSet::UResourceAttributeSet()
{
	InitMaxResource(0.f);
	InitResource(0.f);
	InitResourceGenPer5Seconds(0.f);
}

void UResourceAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UResourceAttributeSet, Resource, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UResourceAttributeSet, MaxResource, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UResourceAttributeSet, ResourceGenPer5Seconds, COND_None, REPNOTIFY_Always);
}

void UResourceAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetResourceAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxResource());
	}
	else if (Attribute == GetMaxResourceAttribute())
	{
		NewValue = FMath::Max(0.f, NewValue);
	}
}

void UResourceAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetMaxResourceAttribute() && GetResource() > NewValue)
	{
		SetResource(NewValue);
	}
}

void UResourceAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetResourceAttribute())
	{
		SetResource(FMath::Clamp(GetResource(), 0.f, GetMaxResource()));
	}
}

void UResourceAttributeSet::OnRep_Resource(const FGameplayAttributeData& OldResource)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UResourceAttributeSet, Resource, OldResource);
}

void UResourceAttributeSet::OnRep_MaxResource(const FGameplayAttributeData& OldMaxResource)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UResourceAttributeSet, MaxResource, OldMaxResource);
}

void UResourceAttributeSet::OnRep_ResourceGenPer5Seconds(const FGameplayAttributeData& OldResourceGenPer5Seconds)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UResourceAttributeSet, ResourceGenPer5Seconds, OldResourceGenPer5Seconds);
}

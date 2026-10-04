#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "ResourceAttributeSet.generated.h"

/** Replicated champion resource values and regeneration rate. */
UCLASS()
class XERSUO_API UResourceAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UResourceAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Resource, Category = "Attributes|Resource")
	FGameplayAttributeData Resource;

	ATTRIBUTE_ACCESSORS_BASIC(UResourceAttributeSet, Resource)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxResource, Category = "Attributes|Resource")
	FGameplayAttributeData MaxResource;

	ATTRIBUTE_ACCESSORS_BASIC(UResourceAttributeSet, MaxResource)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ResourceGenPer5Seconds, Category = "Attributes|Resource")
	FGameplayAttributeData ResourceGenPer5Seconds;

	ATTRIBUTE_ACCESSORS_BASIC(UResourceAttributeSet, ResourceGenPer5Seconds)

protected:
	UFUNCTION()
	void OnRep_Resource(const FGameplayAttributeData& OldResource);

	UFUNCTION()
	void OnRep_MaxResource(const FGameplayAttributeData& OldMaxResource);

	UFUNCTION()
	void OnRep_ResourceGenPer5Seconds(const FGameplayAttributeData& OldResourceGenPer5Seconds);
};

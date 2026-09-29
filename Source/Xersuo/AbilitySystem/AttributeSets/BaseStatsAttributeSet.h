// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "BaseStatsAttributeSet.generated.h"

/** Replicated champion combat stats. */
UCLASS()
class XERSUO_API UBaseStatsAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UBaseStatsAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_AttackDamage, Category = "Attributes|Combat")
	FGameplayAttributeData AttackDamage;

	ATTRIBUTE_ACCESSORS_BASIC(UBaseStatsAttributeSet, AttackDamage)

protected:
	UFUNCTION()
	void OnRep_AttackDamage(const FGameplayAttributeData& OldAttackDamage);
};

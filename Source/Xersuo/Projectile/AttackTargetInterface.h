// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AttackTargetInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UAttackTargetInterface : public UInterface
{
	GENERATED_BODY()
};

class XERSUO_API IAttackTargetInterface
{
	GENERATED_BODY()
	
public:
	virtual FVector GetProjectileTargetLocation() const = 0;
};

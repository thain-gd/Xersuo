#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "NormalAttackConfig.generated.h"

class ATargetedProjectile;
class AActor;
class UAnimMontage;

UENUM(BlueprintType)
enum class ENormalAttackDelivery : uint8
{
	Melee,
	Projectile
};

UCLASS(BlueprintType)
class XERSUO_API UNormalAttackConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	ENormalAttackDelivery Delivery = ENormalAttackDelivery::Melee;

	// The ability can alternate or randomly select an animation.
	// Each montage should send the same attack release anim notify event.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	TArray<TObjectPtr<UAnimMontage>> AttackMontages;

	// Unreal units: centimeters.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", meta = (ClampMin = "0.0", Units = "cm"))
	float BaseAttackRange = 150.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile", meta = (EditCondition = "Delivery == ENormalAttackDelivery::Projectile", EditConditionHides))
	TSubclassOf<ATargetedProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile", meta = (EditCondition = "Delivery == ENormalAttackDelivery::Projectile", EditConditionHides))
	FName ProjectileSpawnSocket = NAME_None;
};
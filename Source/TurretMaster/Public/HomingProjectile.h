#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "HomingProjectile.generated.h"

/**
 * Variant of projectile, which doesn't experience gravity
 * and homes directly towards a projectile
 */
UCLASS()
class TURRETMASTER_API AHomingProjectile : public AProjectile
{
	GENERATED_BODY()

public:
	AHomingProjectile();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret")
	float TurnMultiplier = 1.f;

	virtual void UpdateTargetDest_Implementation() override;
};

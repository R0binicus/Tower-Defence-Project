#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "SmartHomingProjectile.generated.h"

/**
 * Variant of the homing projectile, which gradually 
 * increases its homing factor as it gets closer to 
 * when it thinks it will hit its target
 */
UCLASS()
class TURRETMASTER_API ASmartHomingProjectile : public AProjectile
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Smart Homing Projectile")
	float LifeCountdown;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Smart Homing Projectile")
	float HomingRate = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Smart Homing Projectile")
	TObjectPtr<UCurveFloat> HomingRateCurve;

	bool bHasInitialized = false;

	virtual void UpdateTargetDest_Implementation(float DeltaTime) override;
};

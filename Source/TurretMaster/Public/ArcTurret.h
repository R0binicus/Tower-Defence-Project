#pragma once

#include "CoreMinimal.h"
#include "Turret.h"
#include "ArcTurret.generated.h"

/**
 * Variant of the turret, which shoots it's projectiles in an arc towards 
 * it's intended target, rather than a straight line towards it
 */
UCLASS()
class TURRETMASTER_API AArcTurret : public ATurret
{
	GENERATED_BODY()

protected:
	float AimVerticalUpperBound = 45.f;

	float AimVerticalLowerBound = -35.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Turret")
	bool AngleIsNAN;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
	float BackupAimAngle = 35.f;

	virtual void RotateTowardsEnemy(const float DeltaTime) override;
	 
	virtual float FindDesiredPitch() override;

	virtual void Shoot() override;
};

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
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Turret")
	bool AngleIsNAN;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
	float BackupAimAngle = 20.f;

	virtual float FindDesiredPitch(const FVector& TargetPosition, const FVector& TargetDirection) override;

	virtual void PreBulletSpawnSetValues(const FVector& TargetPosition) override;
};

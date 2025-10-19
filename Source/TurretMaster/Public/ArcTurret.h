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
	virtual float FindDesiredPitch() override;
};

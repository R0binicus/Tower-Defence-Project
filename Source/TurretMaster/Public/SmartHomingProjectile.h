#pragma once

#include "CoreMinimal.h"
#include "HomingProjectile.h"
#include "SmartHomingProjectile.generated.h"

/**
 * Variant of the homing projectile, which gradually 
 * increases its homing factor as it gets closer to 
 * when it thinks it will hit its target
 */
UCLASS()
class TURRETMASTER_API ASmartHomingProjectile : public AHomingProjectile
{
	GENERATED_BODY()

protected:
	virtual void UpdateTargetDest_Implementation() override;
};

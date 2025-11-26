#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "TeslaProjectile.generated.h"

// TODO: Make this into a class which doesn't inherit from AProjectile, and make a 'TurretSpawnable' interface

/**
 * Projectile variant for the tesla turret
 */
UCLASS()
class TURRETMASTER_API ATeslaProjectile : public AProjectile
{
	GENERATED_BODY()
	
};

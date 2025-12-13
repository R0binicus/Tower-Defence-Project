#pragma once

#include "CoreMinimal.h"
#include "Turrets/Turret.h"
#include "SpawnerTurret.generated.h"

/**
 * Turret variant which spawns other objects with set lifetimes
 */
UCLASS()
class TURRETMASTER_API ASpawnerTurret : public ATurret
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
};

#include "Turrets/SpawnerTurret.h"

void ASpawnerTurret::BeginPlay()
{
    ProjectileValues.Lifetime = ShootCooldown;
    ATurret::BeginPlay();
}

#include "ArcTurret.h"

void AArcTurret::RotateTowardsEnemy(const float DeltaTime)
{
    if (!MuzzleDirectionSocket)
    {
        return;
    }

    float TurretDesiredYaw = FindDesiredYaw();
    float TurretDesiredPitch = FindDesiredPitch();

    // Firest set desired rotation variable
    DesiredTurretRotation = FRotator(TurretDesiredPitch, TurretDesiredYaw, InitialRotation.Roll);

    // Then clamp it if it is not allowed
    TurretDesiredPitch = FMath::Clamp(TurretDesiredPitch, AimVerticalLowerBound, AimVerticalUpperBound);
    FRotator ClampedTurretRotation = FRotator(TurretDesiredPitch, TurretDesiredYaw, InitialRotation.Roll);

    // Then set rotation
    FRotator NewRotation = FMath::RInterpTo(CurrentTurretRotation, ClampedTurretRotation, DeltaTime, TurretTurnSpeed);
    SetActorRotation(NewRotation);
}

float AArcTurret::FindDesiredPitch()
{
    // Reset to initial rotation if there is no closest enemy
    if (!CurrentClosestEnemy)
    {
        return InitialRotation.Pitch;
    }

    if (!BulletSpawnPoint)
    {
        return InitialRotation.Pitch;
    }

    // Equation taken from: https://www.forrestthewoods.com/blog/solving_ballistic_trajectories/

    // Get initial values
    const float Speed = ProjectileSpeed;
    const FVector MuzzleLocation = BulletSpawnPoint->GetComponentLocation();
    FVector PlaneTarget = TargetLocation;
    PlaneTarget.Z = MuzzleLocation.Z;
    const float FlatDist = FVector::Distance(MuzzleLocation, PlaneTarget);
    const float HeightDiff = TargetLocation.Z - MuzzleLocation.Z;

    // Calculate the SquareRoot value
    const float SpeedPow2 = Speed * Speed;
    const float SpeedPow4 = SpeedPow2 * SpeedPow2;
    const float FlatDist2 = FlatDist * FlatDist;
    float SquareRoot = (SpeedPow4 - Gravity * ((Gravity * FlatDist2) + (2 * HeightDiff * SpeedPow2)));

    SquareRoot = -sqrt(SquareRoot);

    // If the set projectile velocity is too low, the projectile
    // will be SquareRoot. If so set flag for projectile spawn
    AngleIsNAN = FMath::IsNaN(SquareRoot);
    if (AngleIsNAN)
    {
        return(BackupAimAngle);
    }

    float Angle = atan2((SpeedPow2 + SquareRoot), (Gravity * FlatDist));

    return FMath::RadiansToDegrees(Angle);
}

void AArcTurret::Shoot()
{
    if (!ProjectileClass)
    {
        return;
    }

    if (!World)
    {
        return;
    }

    if (!BulletSpawnPoint)
    {
        return;
    }

    const FRotator SpawnRotation = BulletSpawnPoint->GetComponentRotation();
    const FVector SpawnLocation = BulletSpawnPoint->GetComponentLocation();

    TObjectPtr<AProjectile> Projectile = World->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
    if (!Projectile)
    {
        return;
    }

    // Equation inputs
    const float Height = SpawnLocation.Z - TargetLocation.Z;
    const float AngleRad = FMath::DegreesToRadians(DesiredTurretRotation.Pitch);

    // Set custom projectile velocity if turret was not
    // able to find valid angle with current velocity
    if (AngleIsNAN)
    {
        // Function input pre calculations
        FVector PlaneTarget = TargetLocation;
        PlaneTarget.Z = SpawnLocation.Z;
        const float FlatDistToEnemy = FVector::Distance(SpawnLocation, PlaneTarget);

        ProjectileValues.Speed = CalculateRequiredVelocity(AngleRad, Height, Gravity, FlatDistToEnemy);
    }

    ProjectileValues.PredictedLifetime = CalculateProjectileLifetime(AngleRad, Height, Gravity, ProjectileValues.Speed);

    Projectile->InitializeProjectile(CurrentClosestEnemy, ProjectileValues);
    ShootTimer = ShootCooldown;

    // Reset ProjectileValues if custom projectile velocity was used
    if (AngleIsNAN)
    {
        ProjectileValues.Speed = ProjectileSpeed;
    }
}

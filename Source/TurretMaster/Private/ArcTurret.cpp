#include "ArcTurret.h"

void AArcTurret::RotateTowardsTarget(const float DeltaTime, const FVector& TargetPosition, const FVector& TargetDirection)
{
    if (!MuzzleDirectionSocket)
    {
        return;
    }

    float TurretDesiredYaw = FindDesiredYaw(TargetPosition, TargetDirection);
    float TurretDesiredPitch = FindDesiredPitch(TargetPosition, TargetDirection);

    // Firest set desired rotation variable
    DesiredTurretRotation = FRotator(TurretDesiredPitch, TurretDesiredYaw, InitialRotation.Roll);

    // Then clamp it if it is not allowed
    TurretDesiredPitch = FMath::Clamp(TurretDesiredPitch, AimVerticalLowerBound, AimVerticalUpperBound);
    FRotator ClampedTurretRotation = FRotator(TurretDesiredPitch, TurretDesiredYaw, InitialRotation.Roll);

    // Then set rotation
    FRotator NewRotation = FMath::RInterpTo(CurrentTurretRotation, ClampedTurretRotation, DeltaTime, TurretTurnSpeed);
    SetActorRotation(NewRotation);
}

float AArcTurret::FindDesiredPitch(const FVector& TargetPosition, const FVector& TargetDirection)
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
    FVector PlaneTarget = TargetPosition;
    PlaneTarget.Z = MuzzleLocation.Z;
    const float FlatDist = FVector::Distance(MuzzleLocation, PlaneTarget);
    const float HeightDiff = TargetPosition.Z - MuzzleLocation.Z;

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

void AArcTurret::Shoot(const FVector& TargetPosition)
{
    ATurret::Shoot(TargetPosition);

    // Reset ProjectileValues if custom projectile speed was changed
    if (ProjectileValues.Speed != ProjectileSpeed)
    {
        ProjectileValues.Speed = ProjectileSpeed;
    }
}

void AArcTurret::PreBulletSpawnSetValues(const FVector& TargetPosition)
{
    // Equation inputs
    float Height = BulletSpawnLocation.Z - TargetPosition.Z;
    float AngleRad = FMath::DegreesToRadians(DesiredTurretRotation.Pitch);

    // Set custom projectile velocity if turret was not
    // able to find valid angle with current velocity
    if (AngleIsNAN)
    {
        // Function input pre calculations
        FVector PlaneTarget = TargetPosition;
        PlaneTarget.Z = BulletSpawnLocation.Z;
        const float FlatDistToEnemy = FVector::Distance(BulletSpawnLocation, PlaneTarget);

        ProjectileValues.Speed = CalculateRequiredVelocity(AngleRad, Height, Gravity, FlatDistToEnemy);
    }

    ProjectileValues.PredictedLifetime = CalculateProjectileLifetime(AngleRad, Height, Gravity, ProjectileValues.Speed);
}

void AArcTurret::CalculateEnemyFutureLocationValues(const FVector& EnemyPosition, const FVector& EnemyVelocity, const float ProjectileFlightTime, FRotator& OutDesiredRotation)
{
    FVector TargetPosition = PredictEnemyLocation(EnemyPosition, EnemyVelocity, ProjectileFlightTime);
    FVector TargetDirection = GetDirectionToEnemy(TargetPosition, MuzzleBaseLocation);

    OutDesiredRotation = FindDesiredRotation(TargetPosition, TargetDirection);

    PreBulletSpawnSetValues(TargetPosition);
}

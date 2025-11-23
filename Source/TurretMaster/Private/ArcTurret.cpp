#include "ArcTurret.h"

float AArcTurret::FindDesiredPitch(const FVector& TargetPosition, const FVector& TargetDirection)
{
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

    const float Angle = atan2((SpeedPow2 + SquareRoot), (Gravity * FlatDist));

    return FMath::RadiansToDegrees(Angle);
}

void AArcTurret::PreBulletSpawnSetValues(const FVector& TargetPosition)
{
    // Equation inputs
    const float Height = BulletSpawnLocation.Z - TargetPosition.Z;
    const float AngleRad = FMath::DegreesToRadians(DesiredGunRotation.Pitch);

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

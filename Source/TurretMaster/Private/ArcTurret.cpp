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
    FRotator AllowedTurretRotation = FRotator(TurretDesiredPitch, TurretDesiredYaw, InitialRotation.Roll);

    // Then set rotation
    FRotator NewRotation = FMath::RInterpTo(CurrentTurretRotation, AllowedTurretRotation, DeltaTime, TurretTurnSpeed);
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

    // Equasion taken from: https://www.forrestthewoods.com/blog/solving_ballistic_trajectories/

    // Get initial values
    const float Speed = ProjectileSpeed;
    const FVector MuzzleLocation = BulletSpawnPoint->GetComponentLocation();
    FVector PlaneTarget = TargetLocation;
    PlaneTarget.Z = MuzzleLocation.Z;
    const float FlatDist = FVector::Distance(MuzzleLocation, PlaneTarget);
    const float HeightDiff = TargetLocation.Z - MuzzleLocation.Z;

    // Calculate the SquareRoot value
    const float Speed2 = Speed * Speed;
    const float Speed4 = Speed2 * Speed2;
    const float FlatDist2 = FlatDist * FlatDist;
    float SquareRoot = (Speed4 - Gravity * ((Gravity * FlatDist2) + (2 * HeightDiff * Speed2)));
    SquareRoot = -sqrt(SquareRoot);

    const float Angle = atan2((Speed2 + SquareRoot), (Gravity * FlatDist));

    return FMath::RadiansToDegrees(Angle);
}

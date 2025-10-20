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

    // Equasion taken from: https://www.forrestthewoods.com/blog/solving_ballistic_trajectories/

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

    AngleIsNAN = FMath::IsNaN(SquareRoot);
    if (AngleIsNAN)
    {
        // Equasion taken from: https://www.youtube.com/watch?v=K3YrIfX4sbY
        //float NewSpeed = FlatDist / (1.5f * FMath::Cos(BackupAimAngle));
        //float NewSpeed = ((0.5f * Gravity * 2.f * 2.f) - HeightDiff) * 2.f * FMath::Sin(BackupAimAngle);
        float NewSpeed = FlatDist * -Gravity;
        NewSpeed = NewSpeed / (-2 * FMath::Sin(BackupAimAngle) * FMath::Cos(BackupAimAngle));
        NewSpeed = sqrt(NewSpeed) - 300;
        ProjectileValues.Speed = NewSpeed;

        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("NewSpeed: %f"), NewSpeed));
        
        return(BackupAimAngle);
    }
    else
    {
        ProjectileValues.Speed = ProjectileSpeed;
    }


    const float Angle = atan2((SpeedPow2 + SquareRoot), (Gravity * FlatDist));

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
    if (Projectile)
    {
        Projectile->InitializeProjectile(CurrentClosestEnemy, ProjectileValues);
    }
    ShootTimer = ShootCooldown;
}

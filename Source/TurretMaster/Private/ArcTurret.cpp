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
        float AngleRadians = FMath::DegreesToRadians(BackupAimAngle);
        
        float Time = 0.f;
        
        // Equasion taken from: https://physics.stackexchange.com/questions/27992/solving-for-initial-velocity-required-to-launch-a-projectile-to-a-given-destinat
        float NewSpeed = 0.5 * Gravity * FlatDist * FlatDist;
        NewSpeed = NewSpeed / (FlatDist * FMath::Tan(AngleRadians) + -HeightDiff);
        NewSpeed = (1 / FMath::Cos(AngleRadians)) * sqrt(NewSpeed);

        float DeltaX = 67.8;
        float Grav = -9.8;

        ProjectileValues.Speed = NewSpeed;

        return(BackupAimAngle);
    }
    else
    {
        ProjectileValues.Speed = ProjectileSpeed;
    }

    float Angle = atan2((SpeedPow2 + SquareRoot), (Gravity * FlatDist));

    // Equasion will not work if the player is above the turret
    // Equasion taken from: https://www.omnicalculator.com/physics/projectile-motion
    float Time = (pow(Speed * FMath::Sin(Angle), 2)) + (2 * Gravity * -HeightDiff);
    Time = Speed * FMath::Sin(Angle) + sqrt(Time);
    Time = Time / Gravity;

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

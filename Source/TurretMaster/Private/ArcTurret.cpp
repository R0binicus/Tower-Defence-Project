#include "ArcTurret.h"

float AArcTurret::FindDesiredPitch()
{
    // Reset to initial rotation if there is no closest enemy
    if (!CurrentClosestEnemy)
    {
        return InitialRotation.Pitch;
    }

    // Prevent turret from aiming vertically  
    // if the vertical distance is too great
    /*if (TargetDotProduct < GiveUpVerticalAimThreshold)
    {
        return InitialRotation.Pitch;
    }*/

    // Equasion taken from: https://www.forrestthewoods.com/blog/solving_ballistic_trajectories/

    // Get initial values
    const float Speed = ProjectileSpeed;
    const FVector MuzzleLocation = MuzzleDirectionSocket->GetComponentLocation();
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

bool AArcTurret::CanShoot()
{
    if (!CurrentClosestEnemy)
    {
        return false;
    }

    if (ShootTimer > 0.f)
    {
        return false;
    }

    //if (TargetDotProduct >= FacingTargetThreshold)
    //{
        //return true;
    //}

    return true;
    //return false;
}

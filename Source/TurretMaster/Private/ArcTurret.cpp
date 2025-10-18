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
    if (TargetDotProduct < GiveUpVerticalAimThreshold)
    {
        return InitialRotation.Pitch;
    }

    const float TurretCurrentPitch = CurrentTurretRotation.Pitch;
    const float TargetPitchDifference = FMath::RadiansToDegrees(TargetDirection.Z - MuzzleForward.Z);
    const float TurretDesiredPitch = TurretCurrentPitch + TargetPitchDifference;

    // Used to make the turret continue partially faceing the enemy, 
    // when the enmy is only just outside the turret's aiming bounds
    return FMath::Clamp(TurretDesiredPitch, AimVerticalLowerBound, AimVerticalUpperBound);
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

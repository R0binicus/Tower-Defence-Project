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

    const float TurretCurrentPitch = CurrentTurretRotation.Pitch;
    const float TargetPitchDifference = FMath::RadiansToDegrees(TargetDirection.Z - MuzzleForward.Z);
    const float TurretDesiredPitch = TurretCurrentPitch + TargetPitchDifference;





    const float V = ProjectileSpeed;
    const float G = 980.f;
    const FVector MuzzleLocation = MuzzleDirectionSocket->GetComponentLocation();
    FVector PlaneTarget = TargetLocation;
    PlaneTarget.Z = MuzzleLocation.Z;

    const float X = FVector::Distance(MuzzleLocation, PlaneTarget);
    const float Y = MuzzleLocation.Z - TargetLocation.Z;


    const float V2 = V * V;
    const float V4 = V2 * V2;
    const float X2 = X * X;
    float SQ = (V4 - G * ((G * X2) + (2.0 * Y * V2)));

    SQ = sqrt(SQ);

    const float Angle = atan2((V*V + SQ), (G * X));

    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Angle: %f"), FMath::RadiansToDegrees(Angle)));

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

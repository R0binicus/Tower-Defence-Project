#include "SmartHomingProjectile.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"

void ASmartHomingProjectile::UpdateTargetDest_Implementation(float DeltaTime)
{
	// Normal BeginPlay doesn't work, so it's being done here instead
	if (!bHasInitialized)
	{
		LifeCountdown = ProjectileValues.PredictedLifetime;
		bHasInitialized = true;
	}

	LifeCountdown = LifeCountdown - DeltaTime;

	const TStrongObjectPtr<AActor> LockedTarget = TargetActor.Pin();

	if (!LockedTarget)
	{
		return;
	}

	FVector TargetDirection = LockedTarget->GetActorLocation() - GetActorLocation();
	TargetDirection.Normalize();

	if (!MovementComponent)
	{
		return;
	}

	if (HomingRateCurve)
	{
		 float CurveTimeInput = UKismetMathLibrary::NormalizeToRange(ProjectileValues.PredictedLifetime - LifeCountdown, 0, ProjectileValues.PredictedLifetime);
		 HomingRate = HomingRateCurve->GetFloatValue(CurveTimeInput);

		 if (ProjectileValues.TurnMultiplier == 0.f)
		 {
			 return;
		 }
		 TargetDirection = FMath::Lerp(MovementComponent->Velocity.GetSafeNormal(), TargetDirection, HomingRate);
		 MovementComponent->Velocity = (TargetDirection * ProjectileValues.Speed * (1 + HomingRate));
	}
}

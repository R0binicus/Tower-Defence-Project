#include "SmartHomingProjectile.h"
#include "Kismet/KismetMathLibrary.h"

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

	if (!HomingRateCurve)
	{
		return;
	}

	float CurveTimeInput = UKismetMathLibrary::NormalizeToRange(ProjectileValues.PredictedLifetime - LifeCountdown, 0, ProjectileValues.PredictedLifetime);
	HomingRate = HomingRateCurve->GetFloatValue(CurveTimeInput);

	if (HomingRate == 0.f)
	{
		return;
	}

	FVector TargetDirection = LockedTarget->GetActorLocation() - GetActorLocation();
	TargetDirection.Normalize();
	FVector CurrentVelocity = CollisionMesh->GetPhysicsLinearVelocity();

	TargetDirection = FMath::Lerp(CurrentVelocity, TargetDirection * CurrentVelocity.Length(), HomingRate * ProjectileValues.TurnMultiplier);
	CollisionMesh->SetPhysicsLinearVelocity(TargetDirection, false);
}

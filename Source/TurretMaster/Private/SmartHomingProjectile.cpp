#include "SmartHomingProjectile.h"
#include "Kismet/KismetMathLibrary.h"

void ASmartHomingProjectile::UpdateTargetDest_Implementation(float DeltaTime)
{
	float LifeCountdown = (ProjectileValues.PredictedLifetime - (ProjectileValues.Lifetime - ProjectileLifetimeTimer));

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

	FVector CurrentVelocity = CollisionMesh->GetPhysicsLinearVelocity();
	FVector TargetDirection = LockedTarget->GetActorLocation() - GetActorLocation();
	TargetDirection.Normalize();
	FVector NewVelocity = TargetDirection * (CurrentVelocity.Length() + Gravity * DeltaTime);

	NewVelocity = FMath::Lerp(CurrentVelocity, NewVelocity, HomingRate * ProjectileValues.TurnMultiplier);
	CollisionMesh->SetPhysicsLinearVelocity(NewVelocity, false);
}

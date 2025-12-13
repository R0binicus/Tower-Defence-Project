#include "Projectiles/SmartHomingProjectile.h"
#include "Enemies/Enemy.h"
#include "Kismet/KismetMathLibrary.h"

void ASmartHomingProjectile::UpdateTargetDest_Implementation(const float DeltaTime)
{
	const float LifeCountdown = (ProjectileValues.PredictedLifetime - (GetWorldTimerManager().GetTimerRemaining(LifetimeTimer)));

	const TStrongObjectPtr<AEnemy> LockedTarget = TargetEnemy.Pin();
	if (!LockedTarget || !HomingRateCurve)
	{
		return;
	}

	if (IDamageable::Execute_IsDead(LockedTarget.Get()))
	{
		return;
	}

	const float CurveTimeInput = UKismetMathLibrary::NormalizeToRange(ProjectileValues.PredictedLifetime - LifeCountdown, 0, ProjectileValues.PredictedLifetime);
	HomingRate = HomingRateCurve->GetFloatValue(CurveTimeInput);

	if (HomingRate == 0.f)
	{
		return;
	}

	const FVector CurrentVelocity = CollisionMesh->GetPhysicsLinearVelocity();
	FVector TargetDirection = LockedTarget->GetActorLocation() - GetActorLocation();
	TargetDirection.Normalize();
	FVector NewVelocity = TargetDirection * (CurrentVelocity.Length() + Gravity * DeltaTime);

	NewVelocity = FMath::Lerp(CurrentVelocity, NewVelocity, HomingRate * ProjectileValues.TurnMultiplier);
	CollisionMesh->SetPhysicsLinearVelocity(NewVelocity, false);
}

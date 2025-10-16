#include "HomingProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"

AHomingProjectile::AHomingProjectile() : AProjectile()
{
	MovementComponent->ProjectileGravityScale = 0.f;
}

void AHomingProjectile::UpdateTargetDest_Implementation()
{
	const TStrongObjectPtr<AActor> LockedTarget = TargetActor.Pin();

	if (!LockedTarget)
	{
		return;
	}

	FVector TargetDirection = LockedTarget->GetActorLocation() - GetActorLocation();
	TargetDirection.Normalize();

	if (ProjectileValues.TurnMultiplier != 1.f)
	{
		TargetDirection = FMath::Lerp(MovementComponent->Velocity.GetSafeNormal(), TargetDirection, ProjectileValues.TurnMultiplier);
	}

	MovementComponent->Velocity = (TargetDirection * ProjectileValues.Speed);
}

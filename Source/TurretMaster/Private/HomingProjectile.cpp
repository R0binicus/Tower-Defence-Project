#include "HomingProjectile.h"

AHomingProjectile::AHomingProjectile() : AProjectile()
{
	CollisionMesh->SetEnableGravity(false);
}

void AHomingProjectile::UpdateTargetDest_Implementation(float DeltaTime)
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
		TargetDirection = FMath::Lerp(CollisionMesh->GetPhysicsLinearVelocity().GetSafeNormal(), TargetDirection, ProjectileValues.TurnMultiplier);
	}

	CollisionMesh->SetPhysicsLinearVelocity(TargetDirection * ProjectileValues.Speed, false);
}

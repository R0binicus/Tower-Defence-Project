#include "HomingProjectile.h"
#include "Enemies/Enemy.h"

AHomingProjectile::AHomingProjectile() : AProjectile()
{
	CollisionMesh->SetEnableGravity(false);
}

void AHomingProjectile::UpdateTargetDest_Implementation(const float DeltaTime)
{
	const TStrongObjectPtr<AEnemy> LockedTarget = TargetEnemy.Pin();
	if (!LockedTarget)
	{
		return;
	}

	if (IDamageable::Execute_IsDead(LockedTarget.Get()))
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

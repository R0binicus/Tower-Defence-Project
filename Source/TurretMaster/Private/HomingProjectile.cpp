#include "HomingProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"

AHomingProjectile::AHomingProjectile() : AProjectile()
{
	MovementComponent->ProjectileGravityScale = 0.f;
}

void AHomingProjectile::MoveToTarget_Implementation()
{
	const TStrongObjectPtr<AActor> LockedTarget = TargetActor.Pin();

	if (!LockedTarget)
	{
		return;
	}

	FVector TargetDirection = LockedTarget->GetActorLocation() - GetActorLocation();
	TargetDirection.Normalize();

	MovementComponent->Velocity = (TargetDirection*1000);
}

#include "Projectiles/TeslaProjectile.h"
#include "NiagaraFunctionLibrary.h"

ATeslaProjectile::ATeslaProjectile()
{

}

void ATeslaProjectile::SetProjectileEnabled(bool bNewEnabled)
{
	bEnabled = bNewEnabled;
}

void ATeslaProjectile::SetupProjectile(AEnemy* Enemy, const FProjectileValues& InProjectileValues)
{
	TargetEnemy = Enemy;
	ProjectileValues = InProjectileValues;
	ProjectileLifetimeTimer = ProjectileValues.Lifetime;
	SetActorScale3D(FVector(ProjectileValues.Scale, ProjectileValues.Scale, ProjectileValues.Scale));
	SetProjectileEnabled(true);
}

void ATeslaProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void ATeslaProjectile::Tick(float DeltaTime)
{
}

void ATeslaProjectile::BounceToTargets(int32 NumberOfTargets)
{
	TArray<TObjectPtr<AEnemy>> AttackedEnemies;
	for (size_t i = 0; i < AllowedTeslaBounces + 1; i++)
	{

	}
}

AEnemy* ATeslaProjectile::BounceToTarget(FVector& StartPosition, const TArray<AEnemy*>& AlreadyAttackedEnemies)
{
	return nullptr;
}


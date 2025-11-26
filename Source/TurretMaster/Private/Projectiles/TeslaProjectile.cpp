#include "Projectiles/TeslaProjectile.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy.h"

ATeslaProjectile::ATeslaProjectile() : AProjectile()
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
	BounceToTargets(AllowedTeslaBounces);
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
	TArray<TObjectPtr<AEnemy>> TargetedEnemies;
	TargetedEnemies.Reserve(NumberOfTargets + 1);

	const TStrongObjectPtr<AEnemy> LockedTarget = TargetEnemy.Pin();
	if (!LockedTarget)
	{
		return;
	}

	FVector PreviousTargetPostion = GetActorLocation();
	CreateLightningVFX(GetActorLocation(), LockedTarget.Get()->GetActorLocation());
	PreviousTargetPostion = LockedTarget.Get()->GetActorLocation();
	TargetedEnemies.Add(LockedTarget.Get());

	for (size_t i = 0; i < AllowedTeslaBounces; i++)
	{
		TObjectPtr<AEnemy> NextTarget = FindClosestEnemy(PreviousTargetPostion, TargetedEnemies);
		if (!NextTarget)
		{
			break;
		}

		CreateLightningVFX(PreviousTargetPostion, NextTarget->GetActorLocation());
		TargetedEnemies.Add(NextTarget);
		PreviousTargetPostion = NextTarget->GetActorLocation();
	}

	for (size_t i = 0; i < TargetedEnemies.Num(); i++)
	{
		IDamageable::Execute_TakeDamage(TargetedEnemies[i], ProjectileValues.Damage/i+1);
	}
}

AEnemy* ATeslaProjectile::FindClosestEnemy(const FVector& CheckOriginPoint, const TArray<AEnemy*>& AlreadyTargetedEnemies)
{
	TObjectPtr<AActor> PotentialClosestEnemy = nullptr;
	float CurrentClosestDistance = INFINITY;

	TArray<AActor*> OutActors;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(EnemyCollisionType);

	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), CheckOriginPoint, ProjectileValues.HitExplosionRadius, ObjectTypes, nullptr, TArray<AActor*>(), OutActors);

	for (size_t i = 0; i < OutActors.Num(); i++)
	{
		if (!OutActors[i] || !OutActors[i]->Implements<UDamageable>())
		{
			continue;
		}

		if (IDamageable::Execute_IsDead(OutActors[i]))
		{
			continue;
		}

		if (AlreadyTargetedEnemies.Contains(OutActors[i]))
		{
			continue;
		}

		const FVector EnemyLocation = OutActors[i]->GetActorLocation();
		const float EnemyDistance = FVector::DistSquared(EnemyLocation, CheckOriginPoint);

		if (EnemyDistance > CurrentClosestDistance)
		{
			continue;
		}

		CurrentClosestDistance = EnemyDistance;
		PotentialClosestEnemy = OutActors[i];
	}

	return Cast<AEnemy>(PotentialClosestEnemy);
}

void ATeslaProjectile::CreateLightningVFX(const FVector& StartPosition, const FVector& EndPosition)
{
	if (!LightningVFX)
	{
		return;
	}

	TObjectPtr<UNiagaraComponent> Lightning = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), LightningVFX, EndPosition, GetActorRotation());
	LightningBeamVFXArray.Add(Lightning);
}


#include "Projectiles/TeslaProjectile.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "Enemy.h"

ATeslaProjectile::ATeslaProjectile() : AProjectile()
{
}

void ATeslaProjectile::SetProjectileEnabled(const bool bNewEnabled)
{
	bEnabled = bNewEnabled;
}

void ATeslaProjectile::SetupProjectile(AEnemy* Enemy, const FProjectileValues& InProjectileValues)
{
	TargetEnemy = Enemy;
	ProjectileValues = InProjectileValues;
	SetProjectileLifetime(ProjectileValues.Lifetime);
	SetActorScale3D(FVector(ProjectileValues.Scale, ProjectileValues.Scale, ProjectileValues.Scale));
	SetProjectileEnabled(true);

	if (ProjectileValues.Bounces > LightningBeamVFXArray.Num())
	{
		MakeLightningVFX(ProjectileValues.Bounces - LightningBeamVFXArray.Num());
	}

	BounceToTargets(ProjectileValues.Bounces);
}

void ATeslaProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void ATeslaProjectile::BounceToTargets(const int32 NumberOfTargets)
{
	TArray<TObjectPtr<AEnemy>> TargetedEnemies;
	TargetedEnemies.Reserve(NumberOfTargets + 1);
	CurrentVFXIndex = 0;

	const TStrongObjectPtr<AEnemy> LockedTarget = TargetEnemy.Pin();
	if (!LockedTarget)
	{
		return;
	}

	FVector PreviousTargetPostion = GetActorLocation();
	UseLightningVFX(GetActorLocation(), LockedTarget.Get()->GetActorLocation());
	PreviousTargetPostion = LockedTarget.Get()->GetActorLocation();
	TargetedEnemies.Add(LockedTarget.Get());

	for (size_t i = 0; i < ProjectileValues.Bounces; i++)
	{
		TObjectPtr<AEnemy> NextTarget = FindClosestEnemy(PreviousTargetPostion, TargetedEnemies);
		if (!NextTarget)
		{
			break;
		}

		UseLightningVFX(PreviousTargetPostion, NextTarget->GetActorLocation());
		TargetedEnemies.Add(NextTarget);
		PreviousTargetPostion = NextTarget->GetActorLocation();
	}

	for (size_t i = 0; i < TargetedEnemies.Num(); i++)
	{
		IDamageable::Execute_TakeDamage(TargetedEnemies[i], ProjectileValues.Damage * BounceDamageReductionMultiplier);
	}
}

AEnemy* ATeslaProjectile::FindClosestEnemy(const FVector& CheckOriginPoint, const TArray<AEnemy*>& AlreadyTargetedEnemies) const
{
	TObjectPtr<AActor> PotentialClosestEnemy = nullptr;
	float CurrentClosestDistance = INFINITY;

	TArray<AActor*> OutActors;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(EnemyCollisionType);

	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), CheckOriginPoint, ProjectileValues.EffectRadius, ObjectTypes, nullptr, TArray<AActor*>(), OutActors);

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

void ATeslaProjectile::MakeLightningVFX(const int NewVFXAmount)
{
	LightningBeamVFXArray.Reserve(LightningBeamVFXArray.Num() + NewVFXAmount);
	for (size_t i = 0; i < NewVFXAmount; i++)
	{
		const TObjectPtr<UNiagaraComponent> LightningVFX = CreateLightningVFX();
		if (!LightningVFX)
		{
			return;
		}
	}
}

UNiagaraComponent* ATeslaProjectile::CreateLightningVFX()
{
	if (!LightningVFXType)
	{
		return nullptr;
	}

	const TObjectPtr<UNiagaraComponent> Lightning = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), LightningVFXType, GetActorLocation(), GetActorRotation(), FVector(1.f), false, false);
	LightningBeamVFXArray.Add(Lightning);

	return Lightning;
}

void ATeslaProjectile::UseLightningVFX(const FVector& StartPosition, const FVector& EndPosition)
{
	if (LightningBeamVFXArray.Num() < 1)
	{
		return;
	}

	if (LightningBeamVFXArray.Num() <= CurrentVFXIndex)
	{
		MakeLightningVFX((CurrentVFXIndex + 1) - LightningBeamVFXArray.Num());
	}

	TObjectPtr<UNiagaraComponent> LightningNiagaraComp = LightningBeamVFXArray[CurrentVFXIndex];
	if (!IsValid(LightningNiagaraComp))
	{
		return;
	}

	LightningNiagaraComp->SetWorldLocation(StartPosition);
	LightningNiagaraComp->SetVariableVec3(FName("BeamEnd"), EndPosition);
	LightningNiagaraComp->ActivateSystem();
	CurrentVFXIndex++;
}

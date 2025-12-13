#include "Projectiles/ExplosiveProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Interfaces/Damageable.h"

void AExplosiveProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	TArray<AActor*> OutActors;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(EnemyCollisionType);

	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), ProjectileValues.EffectRadius, ObjectTypes, nullptr, TArray<AActor*>(), OutActors);

	for (size_t i = 0; i < OutActors.Num(); i++)
	{
		if (!OutActors[i] || !OutActors[i]->Implements<UDamageable>())
		{
			continue;
		}

		IDamageable::Execute_TakeDamage(OutActors[i], ProjectileValues.Damage);
	}

	TObjectPtr<UNiagaraComponent> Explosion = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplosionVFX, GetActorLocation(), GetActorRotation());

	SetProjectileEnabled(false);
}
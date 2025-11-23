#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "ExplosiveProjectile.generated.h"

/**
 * Variant of projectile, which damages enemies in an area when it hits surface or enemy
 */
UCLASS()
class TURRETMASTER_API AExplosiveProjectile : public AProjectile
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ExplosiveProjectile")
	TEnumAsByte<EObjectTypeQuery> EnemyCollsionType = EObjectTypeQuery::ObjectTypeQuery8;

	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
};
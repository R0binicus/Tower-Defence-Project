#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileValues.h"
#include "Projectile.generated.h"

class UStaticMeshComponent;
class UProjectileMovementComponent;

/**
 * Projectile base class, which moves in the direction it was
 * fired from at an initial velocity and Expeirences gravity
 */
UCLASS()
class TURRETMASTER_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AProjectile();

	UFUNCTION(BlueprintCallable, Category = "Projectile",
		meta = (ToolTip = "Gets the damage that the projectile will deal to its target"))
	float GetProjectileDamage() const { return ProjectileValues.Damage; }

	UFUNCTION(BlueprintCallable, Category = "Projectile",
		meta = (ToolTip = "Sets the projectile's initial values"))
	void InitializeProjectile(AActor* Target, const FProjectileValues& InProjectileValues);

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Projectile",
		meta = (ToolTip = "Updates the target's destination. Not used by base projectile, but may be for other projectile types"))
	void UpdateTargetDest();

	virtual void UpdateTargetDest_Implementation();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Projectile")
	TWeakObjectPtr<AActor> TargetActor = nullptr;

	UPROPERTY(VisibleDefaultsOnly, Category = "Projectile")
	TObjectPtr<UStaticMeshComponent> CollisionMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	TObjectPtr<UProjectileMovementComponent> MovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	FProjectileValues ProjectileValues;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};

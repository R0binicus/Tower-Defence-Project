#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileValues.h"
#include "Projectile.generated.h"

class UStaticMeshComponent;

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
		meta = (ToolTip = "Sets the projectile to be enabled or disabled"))
	void SetProjectileEnabled(bool bNewEnabled);

	UFUNCTION(BlueprintCallable, Category = "Projectile",
		meta = (ToolTip = "Returns if the projectile is enabled or disabled"))
	float IsProjectileEnabled() const { return bEnabled; }

	UFUNCTION(BlueprintCallable, Category = "Projectile",
		meta = (ToolTip = "Gets the damage that the projectile will deal to its target"))
	float GetProjectileDamage() const { return ProjectileValues.Damage; }

	UFUNCTION(BlueprintCallable, Category = "Projectile",
		meta = (ToolTip = "Reenabled projectile, and sets its new values"))
	void SetupProjectile(AActor* Target, const FProjectileValues& InProjectileValues);

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Projectile",
		meta = (ToolTip = "Updates the target's destination. Not used by base projectile, but may be for other projectile types"))
	void UpdateTargetDest(float DeltaTime);

	virtual void UpdateTargetDest_Implementation(float DeltaTime);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Projectile")
	TWeakObjectPtr<AActor> TargetActor = nullptr;

	UPROPERTY(VisibleDefaultsOnly, Category = "Projectile")
	TObjectPtr<UStaticMeshComponent> CollisionMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	FProjectileValues ProjectileValues;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Projectile")
	float ProjectileLifetimeTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Projectile")
	bool bEnabled = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret")
	float Gravity;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};

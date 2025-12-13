#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Structs/ProjectileValues.h"
#include "Enemies/Enemy.h"
#include "Projectile.generated.h"

class UStaticMeshComponent;

/**
 * Projectile base class, which moves in the direction it was
 * fired from at an initial velocity and Experiences gravity
 */
UCLASS()
class TURRETMASTER_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AProjectile();

	// Getters & setters
	
	UFUNCTION(BlueprintCallable, Category = "Projectile",
		meta = (ToolTip = "Sets the projectile to be enabled or disabled"))
	virtual void SetProjectileEnabled(bool bNewEnabled);

	UFUNCTION(BlueprintCallable, Category = "Projectile",
		meta = (ToolTip = "Returns if the projectile is enabled or disabled"))
	bool IsProjectileEnabled() const { return bEnabled; }

	UFUNCTION(BlueprintCallable, Category = "Projectile",
		meta = (ToolTip = "Gets the damage that the projectile will deal to its target"))
	float GetProjectileDamage() const { return ProjectileValues.Damage; }

	UFUNCTION(BlueprintCallable, Category = "Projectile",
		meta = (ToolTip = "Reenabled projectile, and sets its new values"))
	virtual void SetupProjectile(AEnemy* Enemy, const FProjectileValues& InProjectileValues);

protected:
	// Variables
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Projectile")
	TWeakObjectPtr<AEnemy> TargetEnemy = nullptr;

	UPROPERTY(VisibleDefaultsOnly, Category = "Projectile")
	TObjectPtr<UStaticMeshComponent> CollisionMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	FProjectileValues ProjectileValues;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	FCollisionProfileName EnemyProfileName = FCollisionProfileName::FCollisionProfileName("Enemy");

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Projectile")
	bool bEnabled = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	float Gravity;

	UPROPERTY(BlueprintReadWrite, Category = "Projectile")
	FTimerHandle LifetimeTimer;

	// Functions

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Projectile",
		meta = (ToolTip = "Sets the projectile's lifetime timer"))
	void SetProjectileLifetime(const float NewLifetime);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Projectile",
		meta = (ToolTip = "Updates the target's destination. Not used by base projectile, but may be for other projectile types"))
	void UpdateTargetDest(const float DeltaTime);

	virtual void UpdateTargetDest_Implementation(const float DeltaTime);

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};

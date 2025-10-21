#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "ProjectileValues.h"
#include "Projectile.h"
#include "Turret.generated.h"

/**
 *
 */
UCLASS()
class TURRETMASTER_API ATurret : public AActor
{
	GENERATED_BODY()
	
public:	
	ATurret();

protected:
	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret")
	TObjectPtr<USphereComponent> RangeSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret")
	TObjectPtr<USceneComponent> BulletSpawnPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret")
	TObjectPtr<USceneComponent> MuzzleDirectionSocket;

	// 
	UPROPERTY(EditAnywhere, Category = "Turret")
	TSubclassOf<class AProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Turret")
	FName EnemyTagName = "Enemy";

	// Begin Play Initialize
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret")
	float Gravity = 980.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret")
	FVector TurretLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret")
	FRotator InitialRotation;

	UPROPERTY()
	TObjectPtr<UWorld> World;

	// Enemy reference
	UPROPERTY()
	TArray<TObjectPtr<AActor>> EnemyRefArray;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Turret")
	TObjectPtr<AActor> CurrentClosestEnemy;

	// Turret Aiming
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
	float TurretRange = 3000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
	float TurretTurnSpeed = 2.f;

	// Turret Aim Restrictions
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret")
	float GiveUpVerticalAimThreshold = 0.8f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret")
	float AimVerticalUpperBound = 30.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret")
	float AimVerticalLowerBound = -35.f;

	// Turret Shooting
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret")
	float ShootCooldown = 1.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Turret")
	float ShootTimer = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret")
	float FacingTargetThreshold = 0.999f;

	// Projectile Values
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret",
		meta = (ToolTip = "Only updates at start of level play"))
	float ProjectileDamage = 25.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret",
		meta = (ToolTip = "Only updates at start of level play"))
	float ProjectileSpeed = 3000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret",
		meta = (ToolTip = "Only updates at start of level play"))
	float ProjectileLifetime = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret",
		meta = (ToolTip = "Only updates at start of level play"))
	float ProjectileTurnMultiplier = 1.f;

	UPROPERTY(BlueprintReadOnly, Category = "Turret")
	FProjectileValues ProjectileValues;

	// Update Turret Values
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Turret")
	FVector MuzzleForward;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Turret")
	FVector TargetLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Turret")
	FVector TargetDirection;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Turret")
	FRotator CurrentTurretRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Turret")
	FRotator DesiredTurretRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Turret")
	float TargetDotProduct;

	// Functions
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Turret",
		meta = (ToolTip = "Returns the closest enemy in the RangeSphere"))
	AActor* GetClosestEnemy();

	virtual void UpdateTurretValues();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Turret",
		meta = (ToolTip = "Modifies DirectionOut Vector parameter and tries to return the direction from the turret to the enemy, returns false and ZeroVector if fails"))
	virtual bool TryGetDirectionToEnemy(const FVector& EnemyPosition, FVector& DirectionOut);

	// Turret rotation
	UFUNCTION(BlueprintCallable, Category = "Turret",
		meta = (ToolTip = "Rotates turret actor to face the enemy using the shortest angle"))
	virtual void RotateTowardsEnemy(const float DeltaTime);

	virtual float FindDesiredYaw();

	virtual float FindDesiredPitch();

	// Shooting
	UFUNCTION(BlueprintCallable, Category = "Turret",
		meta = (ToolTip = "Checks if the gun is in cooldown, is facing the target and the target is within range"))
	virtual bool CanShoot();

	UFUNCTION(BlueprintCallable, Category = "Turret",
		meta = (ToolTip = "Shoots the enemy"))
	virtual void Shoot();

	// Misc Projectile Motion Calculations
	UFUNCTION(BlueprintCallable, Category = "Turret",
		meta = (ToolTip = "Calculates the time it will take for the projectile to hit its target. Will fail if height is negative. Expectes angle in radians"))
	virtual float CalculateProjectileLifetime(const float AngleRad, const float Height, const float InGravity, const float InitialVelocity);

	UFUNCTION(BlueprintCallable, Category = "Turret",
		meta = (ToolTip = "Calculates the initial velocity to hit its the target, with a given angle. Expectes angle in radians"))
	virtual float CalculateRequiredVelocity(const float AngleRad, const float Height, const float InGravity, const float FlatDistToEnemy);
};

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

	// Projectiles
	UPROPERTY(EditAnywhere, Category = "Turret")
	TSubclassOf<class AProjectile> ProjectileClass;

	TArray<TObjectPtr<AProjectile>> ProjectilePool;

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

	// Turret Settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
	bool AllowLocationPrediction = true;

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
	float ShootCooldown = 1.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Turret")
	float ShootTimer = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
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
	FVector MuzzleBaseLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Turret")
	FRotator CurrentTurretRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Turret")
	FRotator DesiredTurretRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Turret")
	FVector BulletSpawnLocation;

	// Functions
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Projectile pool
	UFUNCTION(BlueprintCallable, Category = "Turret",
		meta = (ToolTip = "Makes the specified number of projectiles"))
	void MakeProjectiles(const int NewProjectileAmount);

	UFUNCTION(BlueprintCallable, Category = "Turret",
		meta = (ToolTip = "Creates and initialises a projectile"))
	AProjectile* CreateProjectile();

	UFUNCTION(BlueprintCallable, Category = "Turret",
		meta = (ToolTip = "Returns an unused projectile from the pool. Or nullptr if they are all used"))
	AProjectile* GetUnusedProjectile();

	// Turret update
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Turret",
		meta = (ToolTip = "Returns the closest enemy in the RangeSphere"))
	AActor* GetClosestEnemy();

	virtual void UpdateTurretValues();

	UFUNCTION(BlueprintCallable, Category = "Turret",
		meta = (ToolTip = "Returns the direction to Location A from location B"))
	virtual FVector GetDirectionToEnemy(const FVector& EnemyPosition, const FVector& SourcePosition);

	UFUNCTION(BlueprintCallable, Category = "Turret",
		meta = (ToolTip = "Tries to predict the enemy's future location, using its current position and velocity"))
	virtual FVector PredictEnemyLocation(const FVector& EnemyPosition, const FVector& EnemyVelocity, const float ProjectileFlightTime);

	// Turret rotation
	UFUNCTION(BlueprintCallable, Category = "Turret",
		meta = (ToolTip = "Rotates turret actor to face the target using the shortest angle"))
	virtual void RotateTowardsTarget(const float DeltaTime, const FVector& TargetPosition, const FVector& TargetDirection);

	UFUNCTION(BlueprintCallable, Category = "Turret",
		meta = (ToolTip = "Finds the desired angle the turret needs to shoot, to hit the desired location"))
	virtual FRotator FindDesiredRotation(const FVector& TargetPosition, const FVector& TargetDirection, float& OutDesiredYaw, float& OutDesiredPitch);

	virtual float FindDesiredYaw(const FVector& TargetPosition, const FVector& TargetDirection);

	virtual float FindDesiredPitch(const FVector& TargetPosition, const FVector& TargetDirection);

	// Shooting
	UFUNCTION(BlueprintCallable, Category = "Turret",
		meta = (ToolTip = "Checks if the gun is in cooldown, is facing the target and the target is within range"))
	virtual bool CanShoot();

	UFUNCTION(BlueprintCallable, Category = "Turret",
		meta = (ToolTip = "Shoots the enemy"))
	virtual void Shoot(const FVector& TargetPosition);

	virtual void PreBulletSpawnSetValues(const FVector& TargetPosition);

	UFUNCTION(BlueprintCallable, Category = "Turret",
		meta = (ToolTip = "Calculates the predicted future location of the enemy, and returns the angle needed to hit the future location"))
	virtual void CalculateEnemyFutureLocationValues(const FVector& EnemyPosition, const FVector& EnemyVelocity, const float ProjectileFlightTime, FRotator& OutDesiredRotation);

	// Misc Projectile Motion Calculations
	UFUNCTION(BlueprintCallable, Category = "Turret",
		meta = (ToolTip = "Calculates the time it will take for the projectile to hit its target. Will fail if height is negative. Expectes angle in radians"))
	virtual float CalculateProjectileLifetime(const float AngleRad, const float Height, const float InGravity, const float InitialVelocity);

	UFUNCTION(BlueprintCallable, Category = "Turret",
		meta = (ToolTip = "Calculates the initial velocity to hit its the target, with a given angle. Expectes angle in radians"))
	virtual float CalculateRequiredVelocity(const float AngleRad, const float Height, const float InGravity, const float FlatDistToEnemy);
};

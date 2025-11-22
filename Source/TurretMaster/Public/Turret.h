#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "ProjectileValues.h"
#include "Projectile.h"
#include "Buildable.h"
#include "Turret.generated.h"

class AEnemy;
class UBuildingSubsystem;
class UBuildingDataAsset;

/**
 * Base turret class, which aims in a stright line 
 * towards the target, and ignores gravity
 */
UCLASS()
class TURRETMASTER_API ATurret : public AActor, public IBuildable
{
	GENERATED_BODY()
	
public:	
	ATurret();

	void SetProtectPoint_Implementation(AActor* NewProtectPoint);

protected:
	// Components
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret")
	TObjectPtr<USphereComponent> RangeSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret")
	TObjectPtr<USceneComponent> GunParentComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret")
	TObjectPtr<USceneComponent> BulletSpawnPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret")
	TObjectPtr<USceneComponent> MuzzleDirectionSocket;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Turret",
		meta = (ToolTip = "The point in space which the turret tries to protect. Enemies near this point will get attacked first"))
	TObjectPtr<AActor> TurretProtectPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret")
	TObjectPtr<USkeletalMeshComponent> TurretGunMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret")
	TObjectPtr<UStaticMeshComponent> TurretBaseMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
	TObjectPtr<UAnimSequence> TurretShootAnimation;

	// 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
	TObjectPtr<UBuildingDataAsset> BuildingDataAsset;

	// Projectiles pool
	
	UPROPERTY(EditAnywhere, Category = "Turret")
	int InitialProjectilePoolSize = 5;

	UPROPERTY(EditAnywhere, Category = "Turret")
	TSubclassOf<AProjectile> ProjectileClass;

	TArray<TObjectPtr<AProjectile>> ProjectilePool;

	// Begin Play Initialize
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret")
	float Gravity = 980.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret")
	FVector TurretLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret")
	FRotator InitialRotation;

	UPROPERTY()
	TObjectPtr<UWorld> World;

	UPROPERTY()
	TObjectPtr<UBuildingSubsystem> BuildingSubsystem;

	// Enemy
	
	UPROPERTY(EditDefaultsOnly, Category = "Turret")
	FName EnemyTagName = "Enemy";

	UPROPERTY()
	TArray<TObjectPtr<AActor>> EnemyRefArray;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Turret")
	TObjectPtr<AEnemy> CurrentClosestEnemy;

	// Turret Aiming
	
	UPROPERTY(BlueprintReadWrite, Category = "Turret")
	float TurretRange = 3000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
	TEnumAsByte<ECollisionChannel> TurretSightTraceChannel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
	FCollisionProfileName EnemyProfileName = FCollisionProfileName::FCollisionProfileName("Enemy");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret",
		meta = (ToolTip = "Only updates at start of level play"))
	float ExtraTurretFireMinimumRange = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "Turret",
		meta = (ToolTip = "Only updates at start of level play"))
	float TurretFireMinimumRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
	float TurretTurnSpeed = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
	bool AllowLocationPrediction = true;
	

	// Turret Aim Restrictions

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret")
	bool bUseLineOfSight = true;
	
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

	UPROPERTY(BlueprintReadWrite, Category = "Turret")
	FTimerHandle ShootDelayHandle;

	// Projectile Values

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Turret",
		meta = (ToolTip = "Only updates when the level starts"))
	FProjectileValues ProjectileValues = FProjectileValues(
		25.f,	// Damage
		3000.f, // Speed
		10.f,	// Lifetime
		1.f,	// Scale
		1.f		// Turn Multiplier
	);

	// Update Turret Values
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Turret")
	FVector MuzzleForward;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Turret")
	FVector MuzzleBaseLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Turret")
	FRotator CurrentGunRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Turret")
	FRotator DesiredGunRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Turret")
	FVector BulletSpawnLocation;

	// Functions
	
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnCursorOverBegin(AActor* TouchedActor);

	UFUNCTION()
	void OnCursorOverEnd(AActor* TouchedActor);

	// Projectile pool
	
	UFUNCTION(BlueprintCallable, Category = "Turret",
		meta = (ToolTip = "Makes the specified number of projectiles"))
	void MakeProjectiles(const int NewProjectileAmount);

	UFUNCTION(BlueprintCallable, Category = "Turret",
		meta = (ToolTip = "Creates and initialises a projectile"))
	AProjectile* CreateProjectile();

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Turret",
		meta = (ToolTip = "Returns an unused projectile from the pool. Or nullptr if they are all used"))
	AProjectile* GetUnusedProjectile() const;

	// Turret update
	
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Turret",
		meta = (ToolTip = "Returns the closest enemy in the RangeSphere"))
	AEnemy* GetClosestEnemy() const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Turret",
		meta = (ToolTip = "Returns true if the enemy is in line of sight from the gun muzzle"))
	bool IsEnemyInLOS(const FVector& EnemyLocation) const;

	virtual void UpdateTurretValues();

	UFUNCTION(BlueprintCallable, Category = "Turret",
		meta = (ToolTip = "Returns the direction to Location A from location B"))
	virtual FVector GetDirectionToEnemy(const FVector& EnemyPosition, const FVector& SourcePosition) const;

	UFUNCTION(BlueprintCallable, Category = "Turret",
		meta = (ToolTip = "Tries to predict the enemy's future location, using its current position and velocity"))
	virtual FVector PredictEnemyLocation(const FVector& EnemyPosition, const FVector& EnemyVelocity, const float ProjectileFlightTime) const;

	// Turret rotation
	
	UFUNCTION(BlueprintCallable, Category = "Turret",
		meta = (ToolTip = "Rotates turret actor to face the target using the shortest angle"))
	virtual void RotateTowardsTarget(const float DeltaTime, const FVector& TargetPosition, const FVector& TargetDirection);

	UFUNCTION(BlueprintCallable, Category = "Turret",
		meta = (ToolTip = "Finds the desired angle the turret needs to shoot, to hit the desired location"))
	virtual FRotator FindDesiredRotation(const FVector& TargetPosition, const FVector& TargetDirection);

	virtual float FindDesiredYaw(const FVector& TargetPosition, const FVector& TargetDirection) const;

	virtual float FindDesiredPitch(const FVector& TargetPosition, const FVector& TargetDirection);

	// Shooting
	
	UFUNCTION(BlueprintCallable, Category = "Turret",
		meta = (ToolTip = "Checks if the gun is in cooldown, is facing the target and the target is within range"))
	virtual bool CanShoot() const;

	UFUNCTION(BlueprintCallable, Category = "Turret",
		meta = (ToolTip = "Shoots the enemy"))
	virtual void Shoot(const FVector& TargetPosition);

	// Used to set the values just before bullet creation.
	// This function was split off Shoot() to make it cleaner,
	// and to make it easier to override in ArcTurret
	virtual void PreBulletSpawnSetValues(const FVector& TargetPosition);

	UFUNCTION(BlueprintCallable, Category = "Turret",
		meta = (ToolTip = "Calculates the predicted future location of the enemy, and returns the angle needed to hit the future location"))
	virtual void CalculateEnemyFutureLocationValues(const FVector& EnemyPosition, const FVector& EnemyVelocity, const float ProjectileFlightTime, FRotator& OutDesiredRotation);

	// Misc Projectile Motion Calculations

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Turret",
		meta = (ToolTip = "Calculates the time it will take for the projectile to hit its target. Will give incorrect value if height is negative. Expectes angle in radians"))
	virtual float CalculateProjectileLifetime(const float AngleRad, const float Height, const float InGravity, const float InitialVelocity) const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Turret",
		meta = (ToolTip = "Calculates the initial velocity to hit its the target, with a given angle. Expectes angle in radians"))
	virtual float CalculateRequiredVelocity(const float AngleRad, const float Height, const float InGravity, const float FlatDistToEnemy) const;
};

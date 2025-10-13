// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Projectile.h"
#include "Turret.generated.h"

UCLASS()
class TURRETMASTER_API ATurret : public AActor
{
	GENERATED_BODY()
	
public:	
	ATurret();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret")
	USphereComponent* RangeSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret")
	USceneComponent* MuzzleSocket;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Turret")
	FVector TurretLocation;

	UPROPERTY(EditDefaultsOnly, Category = "Turret")
	TSubclassOf<class AProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Turret")
	FName EnemyTagName = "Enemy";

	UPROPERTY()
	UWorld* World;

	UPROPERTY()
	TArray<TObjectPtr<AActor>> EnemyRefArray;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Turret")
	TObjectPtr<AActor> CurrentClosestEnemy;

	// Editable from inspector
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Turret")
	float TurretRange = 3000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
	float TurretTurnSpeed = 2.f;

	// Turret Shooting
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret")
	float ShootCooldown = 1.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Turret")
	float ShootTimer = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret")
	float FacingTargetThreshold = 0.98f;

	// Update Turret Values
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Turret")
	FVector MuzzleForward;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Turret")
	FVector ClosestEnemyLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Turret")
	FVector ClosestEnemyDirection;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Turret")
	float ClosestEnemyDotProduct;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Turret",
		meta = (ToolTip = "Returns the closest enemy in the RangeSphere"))
	AActor* GetClosestEnemy();

	void UpdateTurretValues();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Turret",
		meta = (ToolTip = "Returns the direction from the turret to the enemy location"))
	FVector GetDirectionToEnemy(const FVector& EnemyPosition);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Turret",
		meta = (ToolTip = "Gets the dot product of two vectors after normalizing them"))
	float GetNormalizedDotProduct(FVector VectorA, FVector VectorB);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Turret",
		meta = (ToolTip = "Gets the cross product of two vectors after normalizing them"))
	FVector GetNormalizedCrossProduct(FVector VectorA, FVector VectorB);

	UFUNCTION(BlueprintCallable, Category = "Turret",
		meta = (ToolTip = "Rotates turret actor to face the enemy using the shortest angle"))
	void RotateTowardsEnemy(const float& DeltaTime);

	UFUNCTION(BlueprintCallable, Category = "Turret",
		meta = (ToolTip = "Checks if the gun is in cooldown, is facing the target and the target is within range"))
	void ShootCheck(const float& DeltaTime);

	UFUNCTION(BlueprintCallable, Category = "Turret",
		meta = (ToolTip = "Shoots the enemy"))
	void Shoot();
};

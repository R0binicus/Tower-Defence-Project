// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Turret.generated.h"

UCLASS()
class TURRETMASTER_API ATurret : public AActor
{
	GENERATED_BODY()
	
public:	
	ATurret();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret")
	USphereComponent* RangeSphere;

	

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret")
	float TurretRange = 3000.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Turret")
	FVector TurretLocation;

	UPROPERTY()
	TArray<TObjectPtr<AActor>> EnemyRefArray;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Turret")
	TObjectPtr<AActor> CurrentClosestEnemy;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable, Category = "Turret",
		meta = (ToolTip = "Returns the closest enemy in the RangeSphere"))
	AActor* GetClosestEnemy();
};

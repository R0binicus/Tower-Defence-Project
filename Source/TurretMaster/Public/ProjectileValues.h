#pragma once

#include "CoreMinimal.h"
#include "ProjectileValues.generated.h"

/**
 * Struct used to store and send the data for each projectile
 */
USTRUCT(BlueprintType)
struct FProjectileValues
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Values")
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Values")
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Values")
	float Lifetime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Values")
	float Scale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Values")
	float TurnMultiplier;

	UPROPERTY(BlueprintReadWrite, Category = "Projectile Values")
	float PredictedLifetime;

	FProjectileValues() : Damage(10.f), Speed(1000.f), Lifetime(10.f), Scale(1.f), TurnMultiplier(1.f), PredictedLifetime(2.f)
	{
	}

	FProjectileValues(const float InDamage, const float InSpeed, const float InLifetime, const float InScale, const float InTurnMultiplier, const float InPredictedLifetime = 2) :
		Damage(InDamage), Speed(InSpeed), Lifetime(InLifetime), Scale(InScale), TurnMultiplier(InTurnMultiplier), PredictedLifetime(InPredictedLifetime)
	{
	}
};

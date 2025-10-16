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
	float TurnMultiplier;

	FProjectileValues() : Damage(10.f), Speed(1000.f), Lifetime(10.f), TurnMultiplier(1.f)
	{

	}

	FProjectileValues(float InDamage, float InSpeed, float InLifetime, float InTurnMultiplier) :
		Damage(InDamage), Speed(InSpeed), Lifetime(InLifetime), TurnMultiplier(InTurnMultiplier)
	{

	}
};

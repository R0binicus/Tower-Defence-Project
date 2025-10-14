#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Damageable.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UDamageable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TURRETMASTER_API IDamageable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Damagable")
	void TakeDamage(float DamageTaken);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Damagable")
	bool IsDead();
};

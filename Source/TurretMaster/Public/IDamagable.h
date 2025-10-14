#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IDamagable.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UIDamagable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TURRETMASTER_API IIDamagable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Damagable")
	void TakeDamage(float DamageTaken);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Damagable")
	bool IsDead();
};

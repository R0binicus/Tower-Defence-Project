#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Buildable.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UBuildable : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for turrets or other actors
 * that are buildable on the Buildable Blocks
 */
class TURRETMASTER_API IBuildable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Buildable",
		meta = (ToolTip = "Sets the point which building tries to protect"))
	void SetProtectPoint(AActor* NewProtectPoint);
	void SetProtectPoint_Implementation(AActor* NewProtectPoint);
};

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Buildable.generated.h"

UINTERFACE(MinimalAPI)
class UBuildable : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for turrets or other actors
 * that are buildable on the Buiildable Blocks
 */
class TURRETMASTER_API IBuildable
{
	GENERATED_BODY()

public:
};

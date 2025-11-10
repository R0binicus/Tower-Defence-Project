#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EnemyWaveData.h"
#include "WaveDataObject.generated.h"

/**
 * An object to wrap around a EnemyWaveData, so it can be used with a pointer
 */
UCLASS()
class TURRETMASTER_API UWaveDataObject : public UObject
{
	GENERATED_BODY()
	
public:
	UWaveDataObject();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WaveDataObject")
	FEnemyWaveData WaveData;
};

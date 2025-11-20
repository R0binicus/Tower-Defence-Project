#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EnemyWaveData.h"
#include "LevelDataAsset.generated.h"

/**
 * Primary data asset for storing level data
 */
UCLASS()
class TURRETMASTER_API ULevelDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LevelDataAsset")
	TArray<FEnemyWaveData> LevelWaveData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LevelDataAsset")
	TSoftObjectPtr<AActor> BuildingProtectPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LevelDataAsset")
	float WavePrepTime = 15.f;
};

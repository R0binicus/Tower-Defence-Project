#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "EnemySpawnArea.h"
#include "EnemyWaveData.generated.h"

USTRUCT(BlueprintType)
struct FEnemyWaveData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyWaveData")
	float WaveDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyWaveData")
	float SpawnPeriod;

	// TODO: Discuss, how do i access this in the editor?
	// TODO: Is there any problem with using int32 for editor accesible stuff?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyWaveData")
	TMap<TSubclassOf<AEnemy>, int32> EnemyAmountMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyWaveData")
	TArray<TObjectPtr<AEnemySpawnArea>> SelectedSpawnAreas;

	FEnemyWaveData() : WaveDelay(0.f), SpawnPeriod(0.f)
	{
	}

	FEnemyWaveData(const float InWaveDelay, const float InSpawnPeriod, const TMap<TSubclassOf<AEnemy>, int32>& InEnemyAmountMap, const TArray<TObjectPtr<AEnemySpawnArea>>& InSelectedSpawnAreas) :
		WaveDelay(InWaveDelay), SpawnPeriod(InSpawnPeriod), EnemyAmountMap(InEnemyAmountMap), SelectedSpawnAreas(InSelectedSpawnAreas)
	{
	}
};

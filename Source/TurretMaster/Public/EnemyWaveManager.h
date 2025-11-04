#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyWaveData.h"
#include "EnemySpawnArea.h"
#include "Enemy.h"
#include "EnemyWaveManager.generated.h"

UCLASS()
class TURRETMASTER_API AEnemyWaveManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnemyWaveManager();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EnemyWaveManager",
		meta = (ToolTip = "Gets the number of enemies remaining in the wave"))
	int32 GetEnemiesRemaining() const { return EnemiesRemaining; };

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EnemyWaveManager",
		meta = (ToolTip = "Gets a reference to the NextWaveTimer"))
	FTimerHandle& GetNextWaveTimer() { return NextWaveTimer; };

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EnemyWaveManager",
		meta = (ToolTip = "Gets current wave number"))
	int32 GetCurrentWaveNum() { return CurrentWaveNum; };

protected:
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyWaveManager")
	//TArray<TObjectPtr<AEnemySpawnArea>> SpawnAreas;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyWaveManager")
	TArray<FEnemyWaveData> EnemyWaveData;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "EnemyWaveManager")
	int32 EnemiesRemaining;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "EnemyWaveManager")
	FTimerHandle NextWaveTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "EnemyWaveManager")
	int32 CurrentWaveNum;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "EnemyWaveManager")
	bool bIsSpawningNewWave;

	// Indivigual wave variables

	UPROPERTY(BlueprintReadWrite, Category = "EnemyWaveManager")
	FTimerHandle EnemySpawnTimer;

	FTimerDelegate EnemySpawnTimerDelegate;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "EnemyWaveManager")
	TArray<TSubclassOf<AEnemy>> PendingEnemyWaveSpawns;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Spawn Area")
	float SpawnRateSeconds = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Spawn Area")
	TSubclassOf<AEnemy> EnemyToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Spawn Area")
	AEnemySpawnArea* SpawnAreaToSpawn; // using TObjectPtr breaks BindUObject 

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "EnemyWaveManager",
		meta = (ToolTip = "Starts the process of spawning a new enemy wave"))
	void TriggerNextWaveSpawning();

	//UFUNCTION(BlueprintCallable,BlueprintPure = false, Category = "EnemyWaveManager",
		//meta = (ToolTip = "Gets random spawn area, and weighted random enemy class"))
	//void GetNextEnemyData(AEnemySpawnArea* OutSpawnArea, TSubclassOf<AEnemy> OutEnemyClass) const;

	UFUNCTION(BlueprintCallable, Category = "EnemyWaveManager",
		meta = (ToolTip = "Spawns a new enemy, as part of the enemy wave spawning sequence"))
	void MakeWaveEnemy();

	UFUNCTION(BlueprintCallable, Category = "EnemyWaveManager",
		meta = (ToolTip = "Spawns a new enemy, not necessaraly part of a wave"))
	void SpawnNewEnemy(AEnemySpawnArea* SpawnArea, TSubclassOf<AEnemy> NewEnemyClass);

	UFUNCTION(BlueprintCallable, Category = "EnemyWaveManager",
		meta = (ToolTip = "______ when all the waves are complete"))
	void WavesComplete();

	// Utility functions

	UFUNCTION(BlueprintCallable, Category = "EnemyWaveManager",
		meta = (ToolTip = "______ when all the waves are complete"))
	TSubclassOf<AEnemy> GetWeightedEnemy();

	template<typename T>
	int32 GetRandomIndexFromArray(const TArray<T>& Array) const;
};

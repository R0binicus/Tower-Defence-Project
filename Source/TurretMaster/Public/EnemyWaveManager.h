#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyWaveData.h"
#include "EnemySpawnArea.h"
#include "Enemy.h"
#include "LimitedRepeatTimer.h"
#include "Subsystems/EnemySubsystem.h"
#include "EnemyWaveManager.generated.h"

UCLASS()
class TURRETMASTER_API AEnemyWaveManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnemyWaveManager();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EnemyWaveManager",
		meta = (ToolTip = "Gets a reference to the NextWaveTimer"))
	FTimerHandle& GetNextWaveTimer() { return NextWaveTimer; };

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EnemyWaveManager",
		meta = (ToolTip = "Gets current wave number"))
	int32 GetCurrentWaveNum() { return CurrentWaveNum; };

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyWaveManager")
	float WavePrepTime = 15.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyWaveManager")
	TArray<FEnemyWaveData> EnemyWaveData;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "EnemyWaveManager")
	int32 EnemiesRemaining;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "EnemyWaveManager")
	FTimerHandle NextWaveTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "EnemyWaveManager")
	int32 CurrentWaveNum = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "EnemyWaveManager")
	bool bIsSpawningNewWave;

	// Indivigual wave variables

	UPROPERTY()
	TObjectPtr<ULimitedRepeatTimer> WaveSpawnTimer;

	UPROPERTY(BlueprintReadWrite, Category = "EnemyWaveManager")
	FEnemyWaveData CurrentWaveData;

	UPROPERTY(BlueprintReadWrite, Category = "EnemyWaveManager")
	int32 CurrentWaveEnemyIndex;

	UPROPERTY(BlueprintReadWrite, Category = "EnemyWaveManager")
	FTimerHandle EnemySpawnTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "EnemyWaveManager")
	TArray<TSubclassOf<AEnemy>> PendingEnemyWaveSpawns;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Spawn Area")
	float SpawnRateSeconds = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Spawn Area")
	AEnemySpawnArea* SpawnAreaToSpawn; // using TObjectPtr breaks BindUObject 

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "EnemyWaveManager",
		meta = (ToolTip = "Starts the process of spawning a new enemy wave"))
	void StartNextWave();

	UFUNCTION(BlueprintCallable, Category = "EnemyWaveManager",
		meta = (ToolTip = "Sets up the PendingEnemyWaveSpawns array. This adds the enemies from the EnemyWaveData, then shuffles the array"))
	void SetupEnemySpawnArray();

	UFUNCTION(BlueprintCallable, Category = "EnemyWaveManager",
		meta = (ToolTip = "Sets up the enemies to spawn over time, or all at once"))
	void SetupEnemySpawning();

	UFUNCTION(BlueprintCallable, Category = "EnemyWaveManager",
		meta = (ToolTip = "Spawns a new enemy, as part of the enemy wave spawning sequence"))
	void MakeWaveEnemy();

	UFUNCTION(BlueprintCallable, Category = "EnemyWaveManager",
		meta = (ToolTip = "Spawns a new enemy, not necessaraly part of a wave"))
	void SpawnNewEnemy(AEnemySpawnArea* SpawnArea, TSubclassOf<AEnemy> NewEnemyClass);

	UFUNCTION(BlueprintCallable, Category = "EnemyWaveManager",
		meta = (ToolTip = "Handles recieving an OnEnemyDeath event, reducing the EnemiesRemaining counter and related logic"))
	void OnEnemyDeathHandler();

	UFUNCTION(BlueprintCallable, Category = "EnemyWaveManager",
		meta = (ToolTip = "Handles what to do after the wave is complete"))
	void WaveComplete();

	UFUNCTION(BlueprintCallable, Category = "EnemyWaveManager",
		meta = (ToolTip = "Starts the waiting period, before the next wave starts"))
	void StartWavePrepStage(int32 WaveIndex);

	UFUNCTION(BlueprintCallable, Category = "EnemyWaveManager",
		meta = (ToolTip = "Skips the prep stage, immidietly starting the next wave"))
	void SkipPrepStage();

	UFUNCTION(BlueprintCallable, Category = "EnemyWaveManager",
		meta = (ToolTip = "Triggers victory when all the waves are complete"))
	void WavesComplete();

	// Utility functions

	template<typename T>
	int32 GetRandomArrayIndex(const TArray<T>& Array) const;

	template<typename T>
	void ShuffleArray(TArray<T>& Array) const;
};

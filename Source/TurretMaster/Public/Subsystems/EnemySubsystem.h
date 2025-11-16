#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "EnemyWaveData.h"
#include "WaveDataObject.h"
#include "LimitedRepeatTimer.h"
#include "GameFramework/TowerDefenceGameState.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/TowerDefenceGameInstance.h"
#include "EnemySubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWaveChanged, UWaveDataObject*, NewWaveData, int32, NewWaveNum);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemiesRemainingChanged, int32, NewEnemiesRemaining);

/**
 * Subsystem for handling enemy and wave information, such as enemies remaining in the level
 */
UCLASS()
class TURRETMASTER_API UEnemySubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "EnemySubsystem")
	FOnWaveChanged OnWaveChanged;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "EnemySubsystem")
	FOnEnemiesRemainingChanged OnEnemiesRemainingChanged;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION(BlueprintCallable, Category = "EnemySubsystem",
		meta = (ToolTip = "Initialises the wave data, then starts the waves"))
	void InitialiseWaves(ULevelDataAsset* LevelData);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EnemySubsystem",
		meta = (ToolTip = "Gets the the data for the current enemy wave"))
	UWaveDataObject* GetCurrentWaveData() const { return CurrentWaveObject; };

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EnemySubsystem",
		meta = (ToolTip = "Gets the number of Enemies Remaining"))
	int32 GetEnemiesRemaining() const { return EnemiesRemaining; };

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EnemySubsystem",
		meta = (ToolTip = "Gets the current wave's number"))
	int32 GetCurrentWaveNum() const { return CurrentWaveNum; };

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EnemySubsystem",
		meta = (ToolTip = "Gets the total number of enemy waves"))
	int32 GetTotalWaveNum() const { return TotalWaveNum; };

	UFUNCTION(BlueprintCallable, Category = "EnemySubsystem",
		meta = (ToolTip = "Changes the number of current money the player has"))
	void SetCurrentWaveData(UWaveDataObject* NewWaveObj, const int32 NewWaveNum);

	UFUNCTION(BlueprintCallable, Category = "EnemySubsystem",
		meta = (ToolTip = "Changes the number of enemies remaining in the level"))
	void SetEnemiesRemaining(const int32 NewEnemiesRemaining);

protected:
	UPROPERTY(BlueprintReadWrite, Category = "EnemySubsystem")
	UWaveDataObject* CurrentWaveObject;

	UPROPERTY(BlueprintReadWrite, Category = "EnemySubsystem")
	FEnemyWaveData CurrentWaveData;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "EnemySubsystem")
	int32 EnemiesRemaining;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "EnemySubsystem")
	int32 CurrentWaveNum = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "EnemySubsystem")
	int32 TotalWaveNum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LevelDataAsset")
	float WavePrepTime = 15.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "EnemySubsystem")
	TArray<FEnemyWaveData> WaveDataArray;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "EnemySubsystem")
	TArray<TObjectPtr<UWaveDataObject>> WaveDataObjects;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "EnemySubsystem")
	FTimerHandle NextWaveTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "EnemySubsystem")
	bool bIsSpawningNewWave;

	// Individual wave variables

	UPROPERTY()
	TObjectPtr<ULimitedRepeatTimer> WaveSpawnTimer;

	UPROPERTY(BlueprintReadWrite, Category = "EnemySubsystem")
	int32 CurrentWaveEnemyIndex;

	UPROPERTY(BlueprintReadWrite, Category = "EnemySubsystem")
	FTimerHandle EnemySpawnTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "EnemySubsystem")
	TArray<TSubclassOf<AEnemy>> PendingEnemyWaveSpawns;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "EnemySubsystem")
	AEnemySpawnArea* SpawnAreaToSpawn; // using TObjectPtr breaks BindUObject 

	UFUNCTION(BlueprintCallable, Category = "EnemySubsystem",
		meta = (ToolTip = "Creates an array of WaveDataObjects from an array of EnemyWaveData"))
	TArray<UWaveDataObject*> MakeWaveObjectArray(const TArray<FEnemyWaveData>& NewWaveDataArray);

	UFUNCTION(BlueprintCallable, Category = "EnemySubsystem",
		meta = (ToolTip = "Starts the process of spawning a new enemy wave"))
	void StartNextWave();

	UFUNCTION(BlueprintCallable, Category = "EnemySubsystem",
		meta = (ToolTip = "Sets up the PendingEnemyWaveSpawns array. This adds the enemies from the EnemyWaveData, then shuffles the array"))
	void SetupEnemySpawnArray();

	UFUNCTION(BlueprintCallable, Category = "EnemySubsystem",
		meta = (ToolTip = "Sets up the enemies to spawn over time, or all at once"))
	void SetupEnemySpawning();

	UFUNCTION(BlueprintCallable, Category = "EnemySubsystem",
		meta = (ToolTip = "Spawns a new enemy, as part of the enemy wave spawning sequence"))
	void MakeWaveEnemy();

	UFUNCTION(BlueprintCallable, Category = "EnemySubsystem",
		meta = (ToolTip = "Spawns a new enemy, not necessaraly part of a wave"))
	void SpawnNewEnemy(AEnemySpawnArea* SpawnArea, TSubclassOf<AEnemy> NewEnemyClass);

	UFUNCTION(BlueprintCallable, Category = "EnemySubsystem",
		meta = (ToolTip = "Handles recieving an OnEnemyDeath event, reducing the EnemiesRemaining counter and related logic"))
	void OnEnemyDeathHandler(int32 ResourcesGained);

	UFUNCTION(BlueprintCallable, Category = "EnemySubsystem",
		meta = (ToolTip = "Handles what to do after the wave is complete"))
	void WaveComplete();

	UFUNCTION(BlueprintCallable, Category = "EnemySubsystem",
		meta = (ToolTip = "Starts the waiting period, before the next wave starts"))
	void StartWavePrepStage(int32 WaveIndex);

	UFUNCTION(BlueprintCallable, Category = "EnemySubsystem",
		meta = (ToolTip = "Skips the prep stage, immidietly starting the next wave"))
	void SkipPrepStage();

	UFUNCTION(BlueprintCallable, Category = "EnemySubsystem",
		meta = (ToolTip = "Triggers victory when all the waves are complete"))
	void AllWavesComplete();

	// Utility functions

	template<typename T>
	int32 GetRandomArrayIndex(const TArray<T>& Array) const;

	template<typename T>
	void ShuffleArray(TArray<T>& Array) const;
};

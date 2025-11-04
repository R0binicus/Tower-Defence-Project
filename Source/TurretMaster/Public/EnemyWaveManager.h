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

	// TODO: Discuss, I could imagine a problem with this system being that a 
	// new wave being triggered could override the previous, if it wasn't done.
	// Realistically, don't think this is a problem for this project. 
	// However how might one solve this? Have wave spawner class, that is created
	// to spawn a wave, then deletes itself (or disabled itself for an object pool)
	// that way two waves could be apwned at once? maybe some type of queue

	UFUNCTION(BlueprintCallable, Category = "EnemyWaveManager",
		meta = (ToolTip = "Starts the process of spawning a new enemy wave"))
	void TriggerNextWaveSpawning();

	// TODO: Discuss, how could I make this have convenient out reference   
	// output nodes for a blueprint?

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

	// TODO: Discuss, template doesn't work in blueprints right?

	UFUNCTION(BlueprintCallable, Category = "EnemyWaveManager",
		meta = (ToolTip = "______ when all the waves are complete"))
	TSubclassOf<AEnemy> GetWeightedEnemy();

	template<typename T>
	int32 GetRandomIndexFromArray(const TArray<T>& Array) const;
};

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyWaveData.h"
#include "EnemyWaveManager.generated.h"

UCLASS()
class TURRETMASTER_API AEnemyWaveManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnemyWaveManager();

protected:

	

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyWaveManager")
	//TArray<TObjectPtr<AEnemySpawnArea>> SelectedSpawnAreas;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyWaveManager")
	TArray<FEnemyWaveData> EnemyWaveData;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "EnemyWaveManager")
	int32 EnemiesRemaining;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "EnemyWaveManager")
	float NextWaveCountdown;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "EnemyWaveManager")
	int32 CurrentWaveNum;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "EnemyWaveManager")
	bool bIsSpawningNewWave;

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
	void TriggerNextWaveSpawn() const;

	UFUNCTION(BlueprintCallable, Category = "EnemyWaveManager",
		meta = (ToolTip = "Spawns a new enemy, as part of the enemy wave spawning sequence"))
	void MakeWaveEnemy() const;

	UFUNCTION(BlueprintCallable, Category = "EnemyWaveManager",
		meta = (ToolTip = "Spawns a new enemy, not necessaraly part of a wave"))
	void SpawnNewEnemy() const;

	UFUNCTION(BlueprintCallable, Category = "EnemyWaveManager",
		meta = (ToolTip = "______ when all the waves are complete"))
	void WavesComplete() const;
};

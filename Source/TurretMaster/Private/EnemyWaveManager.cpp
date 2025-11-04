#include "EnemyWaveManager.h"

AEnemyWaveManager::AEnemyWaveManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemyWaveManager::BeginPlay()
{
	Super::BeginPlay();

	// TODO, Discuss BeginPlay exceution order issue
	auto TestStartWave = [this]()
		{
			TriggerNextWaveSpawning();
		};
	FTimerHandle TestStartWaveTimer;
	GetWorldTimerManager().ClearTimer(TestStartWaveTimer);
	TestStartWaveTimer = GetWorldTimerManager().SetTimerForNextTick(TestStartWave);
}

void AEnemyWaveManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemyWaveManager::TriggerNextWaveSpawning()
{
	FEnemyWaveData& WaveData = EnemyWaveData[0];

	int32 NewWaveNumber = 0;
	PendingEnemyWaveSpawns.Empty();
	for (const TPair<TSubclassOf<AEnemy>, int32>& Pair : WaveData.EnemyAmounts)
	{
		int32 Number = Pair.Value;
		PendingEnemyWaveSpawns.Reserve(PendingEnemyWaveSpawns.Num() + Number);
		for (size_t i = 0; i < Number; i++)
		{
			PendingEnemyWaveSpawns.Add(Pair.Key);
		}
	}

	for (size_t i = 0; i < PendingEnemyWaveSpawns.Num(); i++)
	{
		int32 SpawnAreaIndex = GetRandomIndexFromArray(WaveData.SelectedSpawnAreas);
		if (SpawnAreaIndex == -1)
		{
			return;
		}

		int32 EnemyClassIndex = GetRandomIndexFromArray(PendingEnemyWaveSpawns);
		if (EnemyClassIndex == -1)
		{
			return;
		}

		AEnemySpawnArea* NextEnemySpawnArea = WaveData.SelectedSpawnAreas[SpawnAreaIndex];
		TSubclassOf<AEnemy> NextEnemyClass = PendingEnemyWaveSpawns[EnemyClassIndex];

		PendingEnemyWaveSpawns.RemoveAtSwap(EnemyClassIndex);
		i--;

		SpawnNewEnemy(NextEnemySpawnArea, NextEnemyClass);
	}

	//GetNextEnemyData(NextEnemySpawnArea, NextEnemyClass);
	//EnemySpawnTimerDelegate.BindUObject(this, &AEnemyWaveManager::MakeWaveEnemy, SpawnAreaToSpawn, EnemyToSpawn);
	//GetWorldTimerManager().SetTimer(EnemySpawnTimer, EnemySpawnTimerDelegate, SpawnRateSeconds, false);
}

//void AEnemyWaveManager::GetNextEnemyData(AEnemySpawnArea* OutSpawnArea, TSubclassOf<AEnemy> OutEnemyClass) const
//{
	//OutSpawnArea
//}

void AEnemyWaveManager::MakeWaveEnemy()
{

}

void AEnemyWaveManager::SpawnNewEnemy(AEnemySpawnArea* SpawnArea, TSubclassOf<AEnemy> NewEnemyClass)
{
	if (!SpawnArea)
	{
		return;
	}

	SpawnArea->SpawnEnemy(NewEnemyClass);
}

void AEnemyWaveManager::WavesComplete()
{

}

TSubclassOf<AEnemy> AEnemyWaveManager::GetWeightedEnemy()
{
	return TSubclassOf<AEnemy>();
}

template<typename T>
int32 AEnemyWaveManager::GetRandomIndexFromArray(const TArray<T>& Array) const
{
	if (Array.IsEmpty())
	{
		return -1;
	}

	int RandomIndex = FMath::RandRange(0, Array.Num() - 1);
	return RandomIndex;
}

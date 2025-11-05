#include "EnemyWaveManager.h"

AEnemyWaveManager::AEnemyWaveManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemyWaveManager::BeginPlay()
{
	Super::BeginPlay();

	if (!EnemyWaveData.IsValidIndex(0))
	{
		return;
	}

	float FirstWaveDelay = EnemyWaveData[0].WaveDelay;
	if (FirstWaveDelay == 0)
	{
		FirstWaveDelay = 0.001;
	}

	GetWorldTimerManager().SetTimer(EnemySpawnTimer, this, &AEnemyWaveManager::TriggerNextWaveSpawning, FirstWaveDelay, false);
}

void AEnemyWaveManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemyWaveManager::TriggerNextWaveSpawning()
{
	int32 CurrentWaveIndex = CurrentWaveNum;
	CurrentWaveNum++;

	CurrentWaveData = EnemyWaveData[CurrentWaveIndex];
	CurrentWaveEnemyIndex = 0;

	if (EnemyWaveData.Num() > CurrentWaveNum)
	{
		float NextWaveDelay = EnemyWaveData[CurrentWaveIndex + 1].WaveDelay;
		if (NextWaveDelay == 0)
		{
			NextWaveDelay = 0.001;
		}

		GetWorldTimerManager().SetTimer(EnemySpawnTimer, this, &AEnemyWaveManager::TriggerNextWaveSpawning, NextWaveDelay, false);
	}

	PendingEnemyWaveSpawns.Empty();
	for (const TPair<TSubclassOf<AEnemy>, int32>& Pair : CurrentWaveData.EnemyAmounts)
	{
		int32 Number = Pair.Value;
		PendingEnemyWaveSpawns.Reserve(PendingEnemyWaveSpawns.Num() + Number);
		for (size_t i = 0; i < Number; i++)
		{
			PendingEnemyWaveSpawns.Add(Pair.Key);
		}
	}
	ShuffleArray(PendingEnemyWaveSpawns);

	FTimerDelegate TimerDelagate;
	TimerDelagate.BindUObject(this, &AEnemyWaveManager::MakeWaveEnemy);

	WaveSpawnTimer = NewObject<ULimitedRepeatTimer>();
	if (!WaveSpawnTimer)
	{
		return;
	}

	if (CurrentWaveData.SpawnPeriod == 0.f)
	{
		// MakeWaveEnemy increments CurrentWaveEnemyIndex
		while (CurrentWaveEnemyIndex <= PendingEnemyWaveSpawns.Num() - 1)
		{
			MakeWaveEnemy();
		}
		return;
	}

	float DelayBetweenEnemySpawn = CurrentWaveData.SpawnPeriod / PendingEnemyWaveSpawns.Num();
	WaveSpawnTimer->SetupTimer(GetWorld(), TimerDelagate, DelayBetweenEnemySpawn, PendingEnemyWaveSpawns.Num() - 1);
}

//void AEnemyWaveManager::GetNextEnemyData(AEnemySpawnArea* OutSpawnArea, TSubclassOf<AEnemy> OutEnemyClass) const
//{
	//OutSpawnArea
//}

void AEnemyWaveManager::MakeWaveEnemy()
{
	int32 SpawnAreaIndex = GetRandomArrayIndex(CurrentWaveData.SelectedSpawnAreas);
	if (SpawnAreaIndex == -1)
	{
		return;
	}

	if (!PendingEnemyWaveSpawns.IsValidIndex(CurrentWaveEnemyIndex))
	{
		return;
	}

	AEnemySpawnArea* NextEnemySpawnArea = CurrentWaveData.SelectedSpawnAreas[SpawnAreaIndex];
	TSubclassOf<AEnemy> NextEnemyClass = PendingEnemyWaveSpawns[CurrentWaveEnemyIndex];

	SpawnNewEnemy(NextEnemySpawnArea, NextEnemyClass);

	CurrentWaveEnemyIndex++;
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
int32 AEnemyWaveManager::GetRandomArrayIndex(const TArray<T>& Array) const
{
	if (Array.IsEmpty())
	{
		return -1;
	}

	int RandomIndex = FMath::RandRange(0, Array.Num() - 1);
	return RandomIndex;
}

template<typename T>
void AEnemyWaveManager::ShuffleArray(TArray<T>& Array) const
{
	int32 LastIndex = Array.Num() - 1;
	for (int32 i = 0; i <= LastIndex; ++i)
	{
		int32 RandIndex = FMath::RandRange(i, LastIndex);
		if (i != RandIndex)
		{
			Array.Swap(i, RandIndex);
		}
	}
}

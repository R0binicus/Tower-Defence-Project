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

	StartWavePrepStage(0);
}

void AEnemyWaveManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemyWaveManager::StartNextWave()
{
	int32 CurrentWaveIndex = CurrentWaveNum;
	CurrentWaveNum++;

	CurrentWaveData = EnemyWaveData[CurrentWaveIndex];
	CurrentWaveEnemyIndex = 0;

	/*if (EnemyWaveData.Num() > CurrentWaveNum)
	{
		float NextWaveDelay = EnemyWaveData[CurrentWaveIndex + 1].WaveDelay;
		if (NextWaveDelay == 0)
		{
			NextWaveDelay = UE_KINDA_SMALL_NUMBER;
		}

		GetWorldTimerManager().SetTimer(EnemySpawnTimer, this, &AEnemyWaveManager::WaveComplete, NextWaveDelay, false);
	}*/

	SetupEnemySpawnArray();
	SetupEnemySpawning();
}

void AEnemyWaveManager::SetupEnemySpawnArray()
{
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
}

void AEnemyWaveManager::SetupEnemySpawning()
{
	FTimerDelegate TimerDelagate;
	TimerDelagate.BindUObject(this, &AEnemyWaveManager::MakeWaveEnemy);

	WaveSpawnTimer = NewObject<ULimitedRepeatTimer>();
	if (!WaveSpawnTimer)
	{
		return;
	}

	EnemiesRemaining = 0;

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
	WaveSpawnTimer->SetupTimer(GetWorld(), TimerDelagate, DelayBetweenEnemySpawn, PendingEnemyWaveSpawns.Num());
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

	EnemiesRemaining++;
	CurrentWaveEnemyIndex++;
}

void AEnemyWaveManager::SpawnNewEnemy(AEnemySpawnArea* SpawnArea, TSubclassOf<AEnemy> NewEnemyClass)
{
	if (!SpawnArea)
	{
		return;
	}

	TObjectPtr<AEnemy> TempEnemy = SpawnArea->SpawnEnemy(NewEnemyClass);
	TempEnemy->OnEnemyDeath.AddUniqueDynamic(this, &AEnemyWaveManager::OnEnemyDeathHandler);
}

void AEnemyWaveManager::OnEnemyDeathHandler()
{
	EnemiesRemaining--;

	if (EnemiesRemaining > 0)
	{
		return;
	}

	if (EnemyWaveData.Num() <= CurrentWaveNum)
	{
		return;
	}

	WaveComplete();
}

void AEnemyWaveManager::WaveComplete()
{
	StartWavePrepStage(CurrentWaveNum);
}

void AEnemyWaveManager::StartWavePrepStage(int32 WaveIndex)
{
	if (EnemyWaveData.Num() <= CurrentWaveNum)
	{
		WavesComplete();
		return;
	}

	float NextWaveDelay = EnemyWaveData[WaveIndex].WaveDelay + WavePrepTime;
	if (NextWaveDelay <= 0.f)
	{
		NextWaveDelay = UE_KINDA_SMALL_NUMBER;
	}

	GetWorldTimerManager().ClearTimer(EnemySpawnTimer);
	GetWorldTimerManager().SetTimer(EnemySpawnTimer, this, &AEnemyWaveManager::StartNextWave, NextWaveDelay, false);
}

void AEnemyWaveManager::SkipPrepStage()
{
	// Not currently triggered, needs to be set up
	GetWorldTimerManager().ClearTimer(EnemySpawnTimer);
	StartNextWave();
}

void AEnemyWaveManager::WavesComplete()
{
	// Trigger victory contitions
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

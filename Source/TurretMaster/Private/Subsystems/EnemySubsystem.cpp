#include "Subsystems/EnemySubsystem.h"

void UEnemySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UEnemySubsystem::InitializeWaves(TArray<FEnemyWaveData>& WaveData, float NewPrepTime)
{
	if (WaveData.Num() < 1)
	{
		return;
	}

	WavePrepTime = NewPrepTime;
	WaveDataArray = WaveData;
	WaveDataObjects = MakeWaveObjectArray(WaveDataArray);

	int32 StartWaveIndex = 0;

	// If the first wave is fired without any delay, it can cause minor UI issues
	// Howver this shouldn't occur normally, but just in case, here is a quick fix
	float NextWaveDelay = WaveDataArray[StartWaveIndex].WaveDelay + WavePrepTime;
	if (NextWaveDelay > 0.f)
	{
		StartWavePrepStage(StartWaveIndex);
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UEnemySubsystem::StartNextWave);
	}
}

void UEnemySubsystem::SetCurrentWaveData(UWaveDataObject* NewWaveObj, const int32 NewWaveNum)
{
	if (!NewWaveObj)
	{
		return;
	}

	CurrentWaveObject = NewWaveObj;
	CurrentWaveData = CurrentWaveObject->WaveData;
	CurrentWaveNum = NewWaveNum;
 
	OnWaveChanged.Broadcast(CurrentWaveObject, CurrentWaveNum);
}

void UEnemySubsystem::SetEnemiesRemaining(const int32 NewEnemiesRemaining)
{
	EnemiesRemaining = NewEnemiesRemaining;
	OnEnemiesRemainingChanged.Broadcast(EnemiesRemaining);
}

TArray<UWaveDataObject*> UEnemySubsystem::MakeWaveObjectArray(const TArray<FEnemyWaveData>& NewWaveDataArray)
{
	TArray<UWaveDataObject*> WaveObjects;
	WaveObjects.Reserve(NewWaveDataArray.Num());

	for (size_t i = 0; i < NewWaveDataArray.Num(); i++)
	{
		TObjectPtr<UWaveDataObject> WaveObject = NewObject<UWaveDataObject>();
		if (!WaveObject)
		{
			continue;
		}

		WaveObject->WaveData = NewWaveDataArray[i];
		WaveObjects.Add(WaveObject);
	}

	TotalWaveNum = WaveObjects.Num();

	return WaveObjects;
}

void UEnemySubsystem::StartNextWave()
{
	int32 CurrentWaveIndex = CurrentWaveNum;
	CurrentWaveNum++;

	TObjectPtr<UWaveDataObject> WaveObject = WaveDataObjects[CurrentWaveIndex];
	if (!WaveObject)
	{
		return;
	}

	SetCurrentWaveData(WaveObject, CurrentWaveIndex + 1);

	CurrentWaveData = WaveObject->WaveData;
	CurrentWaveEnemyIndex = 0;

	SetupEnemySpawnArray();
	SetupEnemySpawning();
}

void UEnemySubsystem::SetupEnemySpawnArray()
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

void UEnemySubsystem::SetupEnemySpawning()
{
	FTimerDelegate TimerDelagate;
	TimerDelagate.BindUObject(this, &UEnemySubsystem::MakeWaveEnemy);

	WaveSpawnTimer = NewObject<ULimitedRepeatTimer>();
	if (!WaveSpawnTimer)
	{
		return;
	}

	EnemiesRemaining = PendingEnemyWaveSpawns.Num();

	if (CurrentWaveData.SpawnPeriod == 0.f)
	{
		// MakeWaveEnemy increments CurrentWaveEnemyIndex
		while (CurrentWaveEnemyIndex <= PendingEnemyWaveSpawns.Num() - 1)
		{
			MakeWaveEnemy();
		}
	}
	else
	{
		float DelayBetweenEnemySpawn = CurrentWaveData.SpawnPeriod / PendingEnemyWaveSpawns.Num();
		WaveSpawnTimer->SetupTimer(GetWorld(), TimerDelagate, DelayBetweenEnemySpawn, PendingEnemyWaveSpawns.Num());
	}
}

void UEnemySubsystem::MakeWaveEnemy()
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

void UEnemySubsystem::SpawnNewEnemy(AEnemySpawnArea* SpawnArea, TSubclassOf<AEnemy> NewEnemyClass)
{
	if (!SpawnArea)
	{
		return;
	}

	TObjectPtr<AEnemy> TempEnemy = SpawnArea->SpawnEnemy(NewEnemyClass);
	TempEnemy->OnEnemyDeath.AddUniqueDynamic(this, &UEnemySubsystem::OnEnemyDeathHandler);
}

void UEnemySubsystem::OnEnemyDeathHandler()
{
	SetEnemiesRemaining(--EnemiesRemaining);

	if (EnemiesRemaining > 0)
	{
		return;
	}

	if (WaveDataObjects.Num() <= CurrentWaveNum)
	{
		return;
	}

	WaveComplete();
}

void UEnemySubsystem::WaveComplete()
{
	if (WaveDataObjects.Num() <= CurrentWaveNum)
	{
		WavesComplete();
		return;
	}

	StartWavePrepStage(CurrentWaveNum);
	// Also trigger some juice when a wave is complete
}

void UEnemySubsystem::StartWavePrepStage(int32 WaveIndex)
{
	GetWorld()->GetTimerManager().ClearTimer(EnemySpawnTimer);

	TObjectPtr<UWaveDataObject> WaveObject = WaveDataObjects[WaveIndex];
	if (!WaveObject)
	{
		return;
	}

	float NextWaveDelay = WaveObject->WaveData.WaveDelay + WavePrepTime;
	if (NextWaveDelay <= 0.f)
	{
		StartNextWave();
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(EnemySpawnTimer, this, &UEnemySubsystem::StartNextWave, NextWaveDelay, false);
	}
}

void UEnemySubsystem::SkipPrepStage()
{
	// Not currently triggered, needs to be set up
	GetWorld()->GetTimerManager().ClearTimer(EnemySpawnTimer);
	StartNextWave();
}

void UEnemySubsystem::WavesComplete()
{
	// Trigger victory contitions
}

template<typename T>
int32 UEnemySubsystem::GetRandomArrayIndex(const TArray<T>& Array) const
{
	if (Array.IsEmpty())
	{
		return -1;
	}

	int RandomIndex = FMath::RandRange(0, Array.Num() - 1);
	return RandomIndex;
}

template<typename T>
void UEnemySubsystem::ShuffleArray(TArray<T>& Array) const
{
	int32 LastIndex = Array.Num() - 1;
	for (int32 i = 0; i <= LastIndex; ++i)
	{
		int32 RandIndex = FMath::RandRange(i, LastIndex);
		if (i == RandIndex)
		{
			continue;
		}

		Array.Swap(i, RandIndex);
	}
}

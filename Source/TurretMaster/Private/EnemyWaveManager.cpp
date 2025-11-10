#include "EnemyWaveManager.h"

AEnemyWaveManager::AEnemyWaveManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemyWaveManager::BeginPlay()
{
	Super::BeginPlay();

	int32 StartWaveIndex = 0;

	WaveDataObjects = MakeWaveObjectArray(EnemyWaveData);

	// If the first wave is fired without any delay, it can cause minor UI issues
	// Howver this shouldn't occur normally, but just in case, here is a quick fix
	float NextWaveDelay = EnemyWaveData[StartWaveIndex].WaveDelay + WavePrepTime;
	if (NextWaveDelay > 0.f)
	{
		StartWavePrepStage(StartWaveIndex);
	}
	else
	{
		GetWorldTimerManager().SetTimerForNextTick(this, &AEnemyWaveManager::StartNextWave);
	}
}

void AEnemyWaveManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

TArray<UWaveDataObject*> AEnemyWaveManager::MakeWaveObjectArray(const TArray<FEnemyWaveData>& WaveDataArray) const
{
	TArray<UWaveDataObject*> WaveObjects;
	WaveObjects.Reserve(WaveDataArray.Num());

	for (size_t i = 0; i < WaveDataArray.Num(); i++)
	{
		TObjectPtr<UWaveDataObject> WaveObject = NewObject<UWaveDataObject>();
		if (!WaveObject)
		{
			continue;
		}

		WaveObject->WaveData = WaveDataArray[i];
		WaveObjects.Add(WaveObject);
	}

	TObjectPtr<UEnemySubsystem> EnemySubsystem = GetWorld()->GetSubsystem<UEnemySubsystem>();
	if (EnemySubsystem)
	{
		EnemySubsystem->SetTotalWaveNum(WaveObjects.Num());
	}

	return WaveObjects;
}

void AEnemyWaveManager::StartNextWave()
{
	int32 CurrentWaveIndex = CurrentWaveNum;
	CurrentWaveNum++;

	TObjectPtr<UWaveDataObject> WaveObject = WaveDataObjects[CurrentWaveIndex];
	if (!WaveObject)
	{
		return;
	}

	TObjectPtr<UEnemySubsystem> EnemySubsystem = GetWorld()->GetSubsystem<UEnemySubsystem>();
	if (EnemySubsystem)
	{
		EnemySubsystem->SetCurrentWaveData(WaveObject, CurrentWaveIndex + 1);
	}
	
	CurrentWaveData = WaveObject->WaveData;
	CurrentWaveEnemyIndex = 0;

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

	TObjectPtr<AEnemy> TempEnemy = SpawnArea->SpawnEnemy(NewEnemyClass);
	TempEnemy->OnEnemyDeath.AddUniqueDynamic(this, &AEnemyWaveManager::OnEnemyDeathHandler);
}

void AEnemyWaveManager::OnEnemyDeathHandler()
{
	EnemiesRemaining--;

	TObjectPtr<UEnemySubsystem> EnemySubsystem = GetWorld()->GetSubsystem<UEnemySubsystem>();
	if (EnemySubsystem)
	{
		EnemySubsystem->SetEnemiesRemaining(EnemiesRemaining);
	}

	if (EnemySubsystem->GetEnemiesRemaining() > 0)
	{
		return;
	}

	if (WaveDataObjects.Num() <= CurrentWaveNum)
	{
		return;
	}

	WaveComplete();
}

void AEnemyWaveManager::WaveComplete()
{
	StartWavePrepStage(CurrentWaveNum);
	// Also trigger some juice when a wave is complete
}

void AEnemyWaveManager::StartWavePrepStage(int32 WaveIndex)
{
	if (WaveDataObjects.Num() <= CurrentWaveNum)
	{
		WavesComplete();
		return;
	}

	GetWorldTimerManager().ClearTimer(EnemySpawnTimer);

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
		GetWorldTimerManager().SetTimer(EnemySpawnTimer, this, &AEnemyWaveManager::StartNextWave, NextWaveDelay, false);
	}
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

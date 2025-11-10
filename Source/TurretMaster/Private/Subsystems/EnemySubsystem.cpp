#include "Subsystems/EnemySubsystem.h"

void UEnemySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UEnemySubsystem::SetCurrentWaveData(UWaveDataObject* NewWaveData, const int32 NewEnemiesRemaining)
{
	if (!NewWaveData)
	{
		return;
	}

	CurrentWaveData = NewWaveData;
	EnemiesRemaining = NewEnemiesRemaining;
 
	OnWaveChanged.Broadcast(NewWaveData, EnemiesRemaining);
}

void UEnemySubsystem::SetEnemiesRemaining(const int32 NewEnemiesRemaining)
{
	EnemiesRemaining = NewEnemiesRemaining;
	OnEnemiesRemainingChanged.Broadcast(EnemiesRemaining);
}

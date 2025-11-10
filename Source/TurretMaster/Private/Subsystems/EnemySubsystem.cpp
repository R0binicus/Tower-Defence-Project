#include "Subsystems/EnemySubsystem.h"

void UEnemySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UEnemySubsystem::SetCurrentWaveData(UWaveDataObject* NewWaveData, const int32 NewWaveNum)
{
	if (!NewWaveData)
	{
		return;
	}

	CurrentWaveData = NewWaveData;
	CurrentWaveNum = NewWaveNum;
 
	OnWaveChanged.Broadcast(NewWaveData, CurrentWaveNum);
}

void UEnemySubsystem::SetEnemiesRemaining(const int32 NewEnemiesRemaining)
{
	EnemiesRemaining = NewEnemiesRemaining;
	OnEnemiesRemainingChanged.Broadcast(EnemiesRemaining);
}

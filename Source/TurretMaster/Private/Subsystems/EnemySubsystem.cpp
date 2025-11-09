#include "Subsystems/EnemySubsystem.h"

void UEnemySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

//void UEnemySubsystem::SetCurrentWaveData(TObjectPtr<FEnemyWaveData> NewWaveData, const int32 NewEnemiesRemaining)
//{
//	CurrentWaveData = NewWaveData;
//  EnemiesRemaining = NewEnemiesRemaining;
// 
//  OnWaveChanged.Broadcast(CurrentWaveData, EnemiesRemaining);
//}

void UEnemySubsystem::SetEnemiesRemaining(const int32 NewEnemiesRemaining)
{
	EnemiesRemaining = NewEnemiesRemaining;
	OnEnemiesRemainingChanged.Broadcast(EnemiesRemaining);
}

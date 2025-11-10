#include "EnemyWaveManager.h"

AEnemyWaveManager::AEnemyWaveManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemyWaveManager::BeginPlay()
{
	Super::BeginPlay();

	TObjectPtr<UEnemySubsystem> EnemySubsystem = GetWorld()->GetSubsystem<UEnemySubsystem>();
	if (EnemySubsystem)
	{
		EnemySubsystem->InitializeWaves(LevelWaveData, WavePrepTime);
	}
}

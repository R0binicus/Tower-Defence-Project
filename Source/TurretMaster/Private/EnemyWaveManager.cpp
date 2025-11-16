#include "EnemyWaveManager.h"

AEnemyWaveManager::AEnemyWaveManager()
{
}

void AEnemyWaveManager::BeginPlay()
{
	Super::BeginPlay();

	/*TObjectPtr<UEnemySubsystem> EnemySubsystem = GetWorld()->GetSubsystem<UEnemySubsystem>();
	if (EnemySubsystem)
	{
		EnemySubsystem->InitialiseWaves(LevelWaveData, WavePrepTime);
	}

	TObjectPtr<UBuildingSubsystem> BuildingSubsystem = GetWorld()->GetSubsystem<UBuildingSubsystem>();
	if (BuildingSubsystem)
	{
		BuildingSubsystem->SetProtectPoint(BuildingProtectPoint);
	}*/
}

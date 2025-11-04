#include "EnemyWaveManager.h"

AEnemyWaveManager::AEnemyWaveManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemyWaveManager::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemyWaveManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemyWaveManager::TriggerNextWaveSpawn() const
{

}

void AEnemyWaveManager::MakeWaveEnemy() const
{

}

void AEnemyWaveManager::SpawnNewEnemy() const
{

}

void AEnemyWaveManager::WavesComplete() const
{

}

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

void AEnemyWaveManager::TriggerNextWaveSpawn()
{

}

void AEnemyWaveManager::MakeWaveEnemy()
{

}

void AEnemyWaveManager::SpawnNewEnemy()
{

}

void AEnemyWaveManager::WavesComplete()
{

}

TSubclassOf<AEnemy> AEnemyWaveManager::GetWeightedEnemy()
{
	return TSubclassOf<AEnemy>();
}


template<typename T>
int32 AEnemyWaveManager::GetRandomIndexFromArray(const TArray<T>& Array) const
{
	return int32();
}

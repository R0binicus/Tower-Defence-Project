#include "EnemySpawnArea.h"

AEnemySpawnArea::AEnemySpawnArea()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemySpawnArea::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemySpawnArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


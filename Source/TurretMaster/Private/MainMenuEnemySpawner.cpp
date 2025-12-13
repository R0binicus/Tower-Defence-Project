#include "MainMenuEnemySpawner.h"
#include "Components/BoxComponent.h"
#include "Enemies/Enemy.h"

AMainMenuEnemySpawner::AMainMenuEnemySpawner()
{
	SpawnVolumeBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Spawn Volume Box"));
	SpawnVolumeBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMainMenuEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	SpawnVolume = SpawnVolumeBox->GetUnscaledBoxExtent();

    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AMainMenuEnemySpawner::SpawnEnemy, EnemySpawnRate, true);
}

void AMainMenuEnemySpawner::SpawnEnemy()
{
    TSubclassOf<AEnemy> EnemyToSpawn = EnemiesToSpawn[GetRandomArrayIndex(EnemiesToSpawn)];
    if (!EnemyToSpawn)
    {
        return;
    }

    const FRotator CollectibleRotation = FRotator(0.0f, FMath::RandRange(0.0f, 360.0f), 0.0f);

    FVector SpawnLocation = GetActorLocation();
    SpawnLocation.X += FMath::RandRange(-SpawnVolume.X, SpawnVolume.X);
    SpawnLocation.Y += FMath::RandRange(-SpawnVolume.Y, SpawnVolume.Y);
    SpawnLocation.Z += FMath::RandRange(-SpawnVolume.Z, SpawnVolume.Z);

    const TObjectPtr<AEnemy> NewEnemy = GetWorld()->SpawnActor<AEnemy>(EnemyToSpawn, SpawnLocation, CollectibleRotation);
    if (!NewEnemy)
    {
        return;
    }

    if (!EnemyDestination)
    {
        return;
    }

    NewEnemy->SetDestination(EnemyDestination->GetActorLocation());
}

template<typename T>
int32 AMainMenuEnemySpawner::GetRandomArrayIndex(const TArray<T>& Array) const
{
	if (Array.IsEmpty())
	{
		return -1;
	}

	int RandomIndex = FMath::RandRange(0, Array.Num() - 1);
	return RandomIndex;
}

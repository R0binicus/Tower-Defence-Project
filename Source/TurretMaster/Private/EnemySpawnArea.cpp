#include "EnemySpawnArea.h"

AEnemySpawnArea::AEnemySpawnArea()
{
	PrimaryActorTick.bCanEverTick = true;

	SpawnVolumeBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Spawn Volume Box"));
	SpawnVolumeBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SpawnVolumeBox->SetBoxExtent(FVector(1000, 1000, 0), false);
}

void AEnemySpawnArea::BeginPlay()
{
	Super::BeginPlay();

    World = GetWorld();
	SpawnVolume = SpawnVolumeBox->GetUnscaledBoxExtent();

    SpawnEnemy(EnemyToSpawn);
    SpawnEnemy(EnemyToSpawn);
    SpawnEnemy(EnemyToSpawn);
    SpawnEnemy(EnemyToSpawn);
    SpawnEnemy(EnemyToSpawn);
}

void AEnemySpawnArea::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemySpawnArea::SpawnEnemy(const TSubclassOf<AActor> EnemyClass) const
{
    if (!EnemyClass)
    {
        return;
    }

    if (!World)
    {
        return;
    }

    const FRotator CollectibleRotation = FRotator(0.0f, FMath::RandRange(0.0f, 360.0f), 0.0f);

    FVector SpawnLocation = GetActorLocation();
    SpawnLocation.X += FMath::RandRange(-SpawnVolume.X, SpawnVolume.X);
    SpawnLocation.Y += FMath::RandRange(-SpawnVolume.Y, SpawnVolume.Y);
    SpawnLocation.Z += FMath::RandRange(-SpawnVolume.Z, SpawnVolume.Z);

    const TObjectPtr<AActor> NewEnemy = World->SpawnActor<AActor>(EnemyClass, SpawnLocation, CollectibleRotation);
}


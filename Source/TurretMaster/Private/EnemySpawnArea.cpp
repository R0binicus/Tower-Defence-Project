#include "EnemySpawnArea.h"
#include "Components/BoxComponent.h"
#include "Enemy.h"

AEnemySpawnArea::AEnemySpawnArea()
{
	PrimaryActorTick.bCanEverTick = true;

	SpawnVolumeBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Spawn Volume Box"));
	SpawnVolumeBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    // TODO: Remove magic number
	SpawnVolumeBox->SetBoxExtent(FVector(1000, 1000, 0), false);
}

void AEnemySpawnArea::BeginPlay()
{
	Super::BeginPlay();

	SpawnVolume = SpawnVolumeBox->GetUnscaledBoxExtent();
}

void AEnemySpawnArea::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

AEnemy* AEnemySpawnArea::SpawnEnemy(TSubclassOf<AEnemy> NewEnemyClass) const
{
    if (!NewEnemyClass || !EnemyDestination)
    {
        return nullptr;
    }

    const FRotator CollectibleRotation = FRotator(0.0f, FMath::RandRange(0.0f, 360.0f), 0.0f);

    FVector SpawnLocation = GetActorLocation();
    SpawnLocation.X += FMath::RandRange(-SpawnVolume.X, SpawnVolume.X);
    SpawnLocation.Y += FMath::RandRange(-SpawnVolume.Y, SpawnVolume.Y);
    SpawnLocation.Z += FMath::RandRange(-SpawnVolume.Z, SpawnVolume.Z);

    FActorSpawnParameters ActorSpawnParameters;
    ActorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    const TObjectPtr<AEnemy> NewEnemy = GetWorld()->SpawnActor<AEnemy>(NewEnemyClass, SpawnLocation, CollectibleRotation, ActorSpawnParameters);
    if (!NewEnemy)
    {
        return nullptr;
    }

    NewEnemy->SetDestination(EnemyDestination->GetActorLocation());

    return NewEnemy;
}


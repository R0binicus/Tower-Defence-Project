#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "EnemySpawnArea.generated.h"

UCLASS()
class TURRETMASTER_API AEnemySpawnArea : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnemySpawnArea();

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Enemy Spawn Area")
	TObjectPtr<UBoxComponent> SpawnVolumeBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Spawn Area")
	TSubclassOf<AActor> EnemyToSpawn;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy Spawn Area")
	FVector SpawnVolume;

	UPROPERTY()
	TObjectPtr<UWorld> World;

	virtual void BeginPlay() override;

	virtual void Tick(const float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Enemy Spawn Area",
		meta = (ToolTip = "Spawns an enemy class at a random point in the SpawnVolumeBox"))
	void SpawnEnemy(const TSubclassOf<AActor> EnemyClass) const;
};

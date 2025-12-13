#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawnArea.generated.h"

class AEnemy;
class UBoxComponent;

UCLASS()
class TURRETMASTER_API AEnemySpawnArea : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnemySpawnArea();

	// TODO: Make a 'Spawnable' interface
	
	UFUNCTION(BlueprintCallable, Category = "Enemy Spawn Area",
		meta = (ToolTip = "Spawns an enemy class at a random point in the SpawnVolumeBox"))
	AEnemy* SpawnEnemy(TSubclassOf<AEnemy> NewEnemyClass) const;

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Enemy Spawn Area")
	TObjectPtr<UBoxComponent> SpawnVolumeBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy Spawn Area")
	FVector SpawnVolume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Spawn Area")
	TObjectPtr<AActor> EnemyDestination;

	virtual void BeginPlay() override;

	virtual void Tick(const float DeltaTime) override;
};

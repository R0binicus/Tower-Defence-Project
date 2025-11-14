#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Enemy.h"
#include "MainMenuEnemySpawner.generated.h"

/**
 * Simple actor class which spawns enemies infinitely for the main menu
 */
UCLASS()
class TURRETMASTER_API AMainMenuEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AMainMenuEnemySpawner();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enemy Spawn Area")
	TObjectPtr<UBoxComponent> SpawnVolumeBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy Spawn Area")
	FVector SpawnVolume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Spawn Area")
	TObjectPtr<AActor> EnemyDestination;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Spawn Area")
	TArray<TSubclassOf<AEnemy>> EnemiesToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Spawn Area")
	float EnemySpawnRate = 3;

	FTimerHandle TimerHandle;

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "AMainMenuEnemySpawner",
		meta = (ToolTip = "Continuously spawns an enemy"))
	void SpawnEnemy();

	template<typename T>
	int32 GetRandomArrayIndex(const TArray<T>& Array) const;
};

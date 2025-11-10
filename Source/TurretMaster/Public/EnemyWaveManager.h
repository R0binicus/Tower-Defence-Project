#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyWaveData.h"
#include "EnemySpawnArea.h"
#include "WaveDataObject.h"
#include "Enemy.h"
#include "LimitedRepeatTimer.h"
#include "Subsystems/EnemySubsystem.h"
#include "EnemyWaveManager.generated.h"

UCLASS()
class TURRETMASTER_API AEnemyWaveManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnemyWaveManager();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyWaveManager")
	TArray<FEnemyWaveData> LevelWaveData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyWaveManager")
	float WavePrepTime = 15.f;

	virtual void BeginPlay() override;
};

#pragma once

#include "CoreMinimal.h"
#include "EnemyWaveData.h"
#include "Subsystems/EnemySubsystem.h"
#include "Subsystems/BuildingSubsystem.h"
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
	TObjectPtr<AActor> BuildingProtectPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyWaveManager")
	float WavePrepTime = 15.f;

	virtual void BeginPlay() override;
};

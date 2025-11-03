#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyWaveManager.generated.h"

UCLASS()
class TURRETMASTER_API AEnemyWaveManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnemyWaveManager();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
};

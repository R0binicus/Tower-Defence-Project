#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawnArea.generated.h"

UCLASS()
class TURRETMASTER_API AEnemySpawnArea : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnemySpawnArea();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
};

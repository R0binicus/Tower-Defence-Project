#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TowerDefenceGameMode.generated.h"

/**
 * Game Mode for the Tower Defence game
 */
UCLASS()
class TURRETMASTER_API ATowerDefenceGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATowerDefenceGameMode();

	virtual void BeginPlay() override;
};

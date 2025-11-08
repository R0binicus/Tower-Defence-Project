#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UI/PlayerHUD.h"
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

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TowerDefenceGameMode")
	TSubclassOf<UPlayerHUD> PlayerHudClass;

	UPROPERTY(BlueprintReadWrite, Category = "TowerDefenceGameMode")
	TObjectPtr<UPlayerHUD> PlayerHUD;

	virtual void BeginPlay() override;
};

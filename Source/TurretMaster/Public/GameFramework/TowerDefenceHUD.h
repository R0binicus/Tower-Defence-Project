#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/HudWidget.h"
#include "TowerDefenceHUD.generated.h"

/**
 * HUD for the tower defence game.
 */
UCLASS()
class TURRETMASTER_API ATowerDefenceHUD : public AHUD
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TowerDefenceHUD")
	TSubclassOf<UHudWidget> PlayerHudClass;

	UPROPERTY(BlueprintReadWrite, Category = "TowerDefenceHUD")
	TObjectPtr<UHudWidget> HudWidget;
	
	virtual void BeginPlay() override;
};

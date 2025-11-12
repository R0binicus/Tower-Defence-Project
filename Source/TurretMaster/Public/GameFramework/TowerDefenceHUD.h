#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/HudWidget.h"
#include "UI/EndScreenWidget.h"
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
	TSubclassOf<UHudWidget> HudWidgetClass;

	UPROPERTY(BlueprintReadWrite, Category = "TowerDefenceHUD")
	TObjectPtr<UHudWidget> HudWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TowerDefenceHUD")
	TSubclassOf<UEndScreenWidget> VictoryWidgetClass;

	UPROPERTY(BlueprintReadWrite, Category = "TowerDefenceHUD")
	TObjectPtr<UEndScreenWidget> VictoryWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TowerDefenceHUD")
	TSubclassOf<UEndScreenWidget> DefeatWidgetClass;

	UPROPERTY(BlueprintReadWrite, Category = "TowerDefenceHUD")
	TObjectPtr<UEndScreenWidget> DefeatWidget;
	
	virtual void BeginPlay() override;
};

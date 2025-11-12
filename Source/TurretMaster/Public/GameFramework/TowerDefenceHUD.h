#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TowerDefenceHUD.generated.h"

class UHudWidget;
class UEndScreenWidget;

/**
 * HUD for the tower defence game.
 */
UCLASS()
class TURRETMASTER_API ATowerDefenceHUD : public AHUD
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "TowerDefenceHUD",
		meta = (ToolTip = "Makes the victory widget to be visible, or not visible"))
	void SetVictoryWidgetVisible(bool bIsVisible);

	UFUNCTION(BlueprintCallable, Category = "TowerDefenceHUD",
		meta = (ToolTip = "Makes the defeat widget to be visible, or not visible"))
	void SetDefeatWidgetVisible(bool bIsVisible);

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

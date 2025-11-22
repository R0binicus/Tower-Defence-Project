#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BuildingInfoDisplayWidget.generated.h"

class UTextBlock;
class UButton;
class UBuildingSubsystem;
class UBuildingDataAsset;
class ATurret;

/**
 * Widget to display building information
 */
UCLASS()
class TURRETMASTER_API UBuildingInfoDisplayWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(BlueprintReadWrite, Category = "BuildingInfoDisplayWidget", meta = (BindWidget))
	TObjectPtr<UTextBlock> BuildingName;

	UPROPERTY(BlueprintReadWrite, Category = "BuildingInfoDisplayWidget", meta = (BindWidget))
	TObjectPtr<UTextBlock> BuildingDesc;

	UPROPERTY(BlueprintReadOnly, Category = "BuildingInfoDisplayWidget", meta = (BindWidget))
	TObjectPtr<UButton> SellButton;

	// TODO: Change to being in the player state or similar system

	UPROPERTY(BlueprintReadOnly, Category = "BuildingInfoDisplayWidget")
	TObjectPtr<ATurret> SelectedTurret;

	UPROPERTY(BlueprintReadOnly, Category = "BuildingInfoDisplayWidget")
	TObjectPtr<UBuildingDataAsset> SelectedTurretData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
	float SellFraction = 0.5f;

	void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, Category = "BuildingInfoDisplayWidget",
		meta = (ToolTip = "Updates the information in the building info display"))
	void UpdateBuildingInfoDisplay(UBuildingDataAsset* BuildingData, ATurret* Turret);

	UFUNCTION(BlueprintCallable, Category = "BuildingInfoDisplayWidget",
		meta = (ToolTip = "Hides the building display text and buttons"))
	void HideBuildingDisplay();

	UFUNCTION(BlueprintCallable, Category = "BuildingInfoDisplayWidget",
		meta = (ToolTip = "Sells the building that is currently selected"))
	void SellBuildingPressed();
};

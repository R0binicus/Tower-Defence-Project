#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BuildingInfoDisplayWidget.generated.h"

class UTextBlock;
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

	void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, Category = "BuildingInfoDisplayWidget",
		meta = (ToolTip = "Updates the information in the building info display"))
	void UpdateBuildingInfoDisplay(const UBuildingDataAsset* BuildingData, const ATurret* Turret);

	UFUNCTION(BlueprintCallable, Category = "BuildingInfoDisplayWidget",
		meta = (ToolTip = "Hides the building display text and buttons"))
	void HideBuildingDisplay();
};

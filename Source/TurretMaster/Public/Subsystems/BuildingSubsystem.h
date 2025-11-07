#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "DataAssets/BuildingDataAsset.h"
#include "BuildingSubsystem.generated.h"

/**
 * Subsystem for managing resources and building structures
 */
UCLASS()
class TURRETMASTER_API UBuildingSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

	
public:
	UFUNCTION(BlueprintCallable, Category = "BuildingSubsystem",
		meta = (ToolTip = "Sets the building subsystem's CurrentPlaceBuildingSelected variable. Used when a player selects the place building button"))
	void SelectedPlaceBuilding(UBuildingDataAsset* BuildingData);

	UFUNCTION(BlueprintCallable, Category = "BuildingSubsystem",
		meta = (ToolTip = "Clears the CurrentPlaceBuildingSelected"))
	void CancelPlaceBuilding();

	UFUNCTION(BlueprintCallable, Category = "BuildingSubsystem",
		meta = (ToolTip = "Tells the building subsystem that a building is placed, then removes appropriate resources"))
	void BuildingPlaced();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BuildingSubsystem")
	TObjectPtr<UBuildingDataAsset> CurrentPlaceBuildingSelected = nullptr;

};

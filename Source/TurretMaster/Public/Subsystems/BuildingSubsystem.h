#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "BuildingSubsystem.generated.h"

class UBuildingDataAsset;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBuildingTypeSelected, UBuildingDataAsset*, BuildingType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBuildingHighlighted, UBuildingDataAsset*, BuildingData, ATurret*, Turret);

/**
 * Subsystem for managing resources and building structures
 */
UCLASS()
class TURRETMASTER_API UBuildingSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

	
public:
	// Sent when the player selects a new building type to be placed
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "BuildingSubsystem")
	FOnBuildingTypeSelected OnBuildingTypeSelected;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "BuildingSubsystem")
	FOnBuildingHighlighted OnBuildingHighlighted;

	void StartSubsystem();

	UFUNCTION(BlueprintCallable, Category = "BuildingSubsystem",
		meta = (ToolTip = "Returns the point which buildings try to protect"))
	AActor* GetProtectPoint() const { return ProtectPoint; };

	UFUNCTION(BlueprintCallable, Category = "BuildingSubsystem",
		meta = (ToolTip = "Sets the building subsystem's CurrentPlaceBuildingSelected variable. Used when a player selects the place building button"))
	void SelectedPlaceBuilding(UBuildingDataAsset* BuildingData);

	UFUNCTION(BlueprintCallable, Category = "BuildingSubsystem",
		meta = (ToolTip = "Tells the building subsystem that a building is placed, then removes appropriate resources"))
	void BuildingPlaced() const;

	UFUNCTION(BlueprintCallable, Category = "BuildingSubsystem",
		meta = (ToolTip = "Clears the CurrentPlaceBuildingSelected"))
	void CancelPlaceBuilding() const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BuildingSubsystem")
	TObjectPtr<UBuildingDataAsset> CurrentPlaceBuildingSelected = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BuildingSubsystem")
	TObjectPtr<AActor> ProtectPoint;

	UFUNCTION(BlueprintCallable, Category = "BuildingSubsystem",
		meta = (ToolTip = "Sets the point which buildings try to protect using a level data asset"))
	void LoadProtectPoint(ULevelDataAsset* LevelData);

	UFUNCTION()
	void SetProtectPoint(TSoftObjectPtr<AActor> SoftProtectPoint);
};

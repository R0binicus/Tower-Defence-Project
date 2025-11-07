#include "Subsystems/BuildingSubsystem.h"

void UBuildingSubsystem::SelectedPlaceBuilding(UBuildingDataAsset* BuildingData)
{
	if (!BuildingData)
	{
		return;
	}

	CurrentPlaceBuildingSelected = BuildingData;
	OnBuildingTypeSelected.Broadcast(CurrentPlaceBuildingSelected);
}

void UBuildingSubsystem::BuildingPlaced()
{
	CancelPlaceBuilding();
}

void UBuildingSubsystem::CancelPlaceBuilding()
{
	CurrentPlaceBuildingSelected = nullptr;
	OnBuildingTypeSelected.Broadcast(CurrentPlaceBuildingSelected);
}

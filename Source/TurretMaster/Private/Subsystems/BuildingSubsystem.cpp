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

void UBuildingSubsystem::CancelPlaceBuilding()
{
	CurrentPlaceBuildingSelected = nullptr;
}

void UBuildingSubsystem::BuildingPlaced()
{
	CancelPlaceBuilding();
}

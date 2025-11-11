#include "Subsystems/BuildingSubsystem.h"

void UBuildingSubsystem::SetProtectPoint(AActor* NewProtectPoint)
{
	ProtectPoint = NewProtectPoint;
}

void UBuildingSubsystem::SelectedPlaceBuilding(UBuildingDataAsset* BuildingData)
{
	if (!BuildingData)
	{
		return;
	}

	TObjectPtr<ATowerDefencePlayerState> PlayerState = Cast<ATowerDefencePlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0));
	if (!PlayerState)
	{
		return;
	}

	if (!PlayerState->HasEnoughResources(BuildingData->Cost))
	{
		return;
	}

	if (!PlayerState->TrySetPlayerState(EPlayerStateEnum::Building))
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
	TObjectPtr<ATowerDefencePlayerState> PlayerState = Cast<ATowerDefencePlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0));
	if (!PlayerState)
	{
		return;
	}

	PlayerState->TrySetPlayerState(EPlayerStateEnum::Default);
}

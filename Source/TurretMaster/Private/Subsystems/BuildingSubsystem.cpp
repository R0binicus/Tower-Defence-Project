#include "Subsystems/BuildingSubsystem.h"

void UBuildingSubsystem::SelectedPlaceBuilding(UBuildingDataAsset* BuildingData)
{
	if (!BuildingData)
	{
		return;
	}

	// TODO: Discuss, setting CustomPlayerState when Subsystem is created
	TObjectPtr<APlayerState> GenericPlayerState = UGameplayStatics::GetPlayerState(GetWorld(), 0);
	TObjectPtr<ATowerDefencePlayerState> CustomPlayerState = Cast<ATowerDefencePlayerState>(GenericPlayerState);
	if (!CustomPlayerState)
	{
		return;
	}

	if (!CustomPlayerState->TrySetPlayerState(EPlayerStateEnum::Building))
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
	TObjectPtr<APlayerState> GenericPlayerState = UGameplayStatics::GetPlayerState(GetWorld(), 0);
	TObjectPtr<ATowerDefencePlayerState> CustomPlayerState = Cast<ATowerDefencePlayerState>(GenericPlayerState);
	if (!CustomPlayerState)
	{
		return;
	}

	CustomPlayerState->TrySetPlayerState(EPlayerStateEnum::Default);
}

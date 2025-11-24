#include "Subsystems/BuildingSubsystem.h"
#include "DataAssets/BuildingDataAsset.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/TowerDefencePlayerState.h"
#include "GameFramework/TowerDefenceGameInstance.h"
#include "PrimaryAssets/LevelDataAsset.h"

void UBuildingSubsystem::StartSubsystem()
{
	UTowerDefenceGameInstance* GameInstance = Cast<UTowerDefenceGameInstance>(GetWorld()->GetGameInstance());
	if (GameInstance)
	{
		GameInstance->OnLevelDataLoaded.AddUniqueDynamic(this, &UBuildingSubsystem::LoadProtectPoint);
	}
}

void UBuildingSubsystem::SelectedPlaceBuilding(UBuildingDataAsset* BuildingData)
{
	if (!BuildingData)
	{
		return;
	}

	const TObjectPtr<ATowerDefencePlayerState> PlayerState = Cast<ATowerDefencePlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0));
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

void UBuildingSubsystem::BuildingPlaced() const
{
	CancelPlaceBuilding();
}

void UBuildingSubsystem::CancelPlaceBuilding() const
{
	const TObjectPtr<ATowerDefencePlayerState> PlayerState = Cast<ATowerDefencePlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0));
	if (!PlayerState)
	{
		return;
	}

	PlayerState->TrySetPlayerState(EPlayerStateEnum::Default);
}

void UBuildingSubsystem::LoadProtectPoint(ULevelDataAsset* LevelData)
{
	if (!LevelData)
	{
		return;
	}

	FStreamableManager& StreamableManager = UAssetManager::Get().GetStreamableManager();

	TSoftObjectPtr<AActor> SoftProtectPoint = LevelData->BuildingProtectPoint;

	if (SoftProtectPoint.IsNull())
	{
		return; // The data asset probably has an empty entry
	}

	if (!SoftProtectPoint.IsPending())
	{
		SetProtectPoint(SoftProtectPoint); // Object is already loaded
		return;
	}

	FStreamableDelegate SetSpawnerArrayDelegate;
	SetSpawnerArrayDelegate.BindUObject(this, &UBuildingSubsystem::SetProtectPoint, SoftProtectPoint);

	TSharedPtr<FStreamableHandle> Handle = StreamableManager.RequestAsyncLoad(SoftProtectPoint.ToSoftObjectPath(), SetSpawnerArrayDelegate);
}

void UBuildingSubsystem::SetProtectPoint(TSoftObjectPtr<AActor> SoftProtectPoint)
{
	if (!SoftProtectPoint)
	{
		return;
	}

	const TObjectPtr<AActor> NewProtectPoint = SoftProtectPoint.Get();
	if (!NewProtectPoint)
	{
		return;
	}

	ProtectPoint = NewProtectPoint;
}

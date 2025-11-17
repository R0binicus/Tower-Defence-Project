#include "Subsystems/BuildingSubsystem.h"

void UBuildingSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	TObjectPtr<UWorld> World = GetWorld();
	if (!World)
	{
		return;
	}

	UTowerDefenceGameInstance* GameInstance = Cast<UTowerDefenceGameInstance>(World->GetGameInstance());
	if (GameInstance)
	{
		GameInstance->LoadDataUsingLevel(World);
		GameInstance->OnLevelDataLoaded.AddUniqueDynamic(this, &UBuildingSubsystem::LoadProtectPoint);
	}
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

	TObjectPtr<AActor> NewProtectPoint = SoftProtectPoint.Get();
	if (!NewProtectPoint)
	{
		return;
	}

	ProtectPoint = NewProtectPoint;
}

#include "GameFramework/TowerDefenceGameInstance.h"
#include "PrimaryAssets/LevelDataAsset.h"
#include "Engine/AssetManager.h"

void UTowerDefenceGameInstance::LoadDataUsingLevel(const TSoftObjectPtr<UWorld> InWorld)
{
	const FString WorldName = InWorld.GetAssetName();

	if (!LevelDataIdMap.Contains(WorldName))
	{
		return;
	}
	FPrimaryAssetId AssetId = LevelDataIdMap[WorldName];

	UAssetManager* Manager = UAssetManager::GetIfInitialized();
	if (!Manager)
	{
		return;
	}

	if (CurrentLevelData)
	{
		Manager->UnloadPrimaryAsset(CurrentLevelData->GetPrimaryAssetId());
		CurrentLevelData = nullptr;
	}

	const TArray<FName> Bundles;
	const FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(this, &UTowerDefenceGameInstance::OnAssetLoaded, AssetId);

	Manager->LoadPrimaryAsset(AssetId, Bundles, Delegate);
}

UWorld* UTowerDefenceGameInstance::GetLevel() const
{
	return GetWorld();
}

void UTowerDefenceGameInstance::OnAssetLoaded(const FPrimaryAssetId LoadedId)
{
	const UAssetManager* Manager = UAssetManager::GetIfInitialized();
	if (!Manager)
	{
		return;
	}

	const TObjectPtr<UClass> Class = Manager->GetPrimaryAssetObject<UClass>(LoadedId);
	if (!Class)
	{
		return;
	}

	const TObjectPtr<ULevelDataAsset> MyTypedObject = Cast<ULevelDataAsset>(Class->GetDefaultObject());
	if (MyTypedObject)
	{
		CurrentLevelData = MyTypedObject;
		OnLevelDataLoaded.Broadcast(CurrentLevelData);
	}
}

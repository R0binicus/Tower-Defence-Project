#include "GameFramework/TowerDefenceGameInstance.h"

void UTowerDefenceGameInstance::LoadDataUsingLevel(TSoftObjectPtr<UWorld> InWorld)
{
	FString WorldName = InWorld.GetAssetName();

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

	TArray<FName> Bundles;
	FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(this, &UTowerDefenceGameInstance::OnAssetLoaded, AssetId);

	Manager->LoadPrimaryAsset(AssetId, Bundles, Delegate);
}

UWorld* UTowerDefenceGameInstance::GetLevel()
{
	return GetWorld();
}

void UTowerDefenceGameInstance::OnAssetLoaded(FPrimaryAssetId LoadedId)
{
	UAssetManager* Manager = UAssetManager::GetIfInitialized();
	if (!Manager)
	{
		return;
	}

	TObjectPtr<UClass> Class = Manager->GetPrimaryAssetObject<UClass>(LoadedId);
	if (!Class)
	{
		return;
	}

	TObjectPtr<ULevelDataAsset> MyTypedObject = Cast<ULevelDataAsset>(Class->GetDefaultObject());
	if (MyTypedObject)
	{
		CurrentLevelData = MyTypedObject;
		OnLevelDataLoaded.Broadcast(CurrentLevelData);
	}
}

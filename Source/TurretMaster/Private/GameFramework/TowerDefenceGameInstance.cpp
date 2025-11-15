#include "GameFramework/TowerDefenceGameInstance.h"

void UTowerDefenceGameInstance::TestLoadAsset(FPrimaryAssetId AssetId)
{
	UAssetManager* Manager = UAssetManager::GetIfValid();
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

void UTowerDefenceGameInstance::OnAssetLoaded(FPrimaryAssetId LoadedId)
{
	UAssetManager* Manager = UAssetManager::GetIfValid();
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

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("String: %s"), *CurrentLevelData->GetPrimaryAssetId().ToString()));
	}
}

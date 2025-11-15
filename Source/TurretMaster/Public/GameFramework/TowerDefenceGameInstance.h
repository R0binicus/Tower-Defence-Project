#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PrimaryAssets/LevelDataAsset.h"
#include "Engine/AssetManager.h"
#include "TowerDefenceGameInstance.generated.h"

/**
 * Game instance for the tower defence game
 * Primarily handles loading if level data for each level
 */
UCLASS()
class TURRETMASTER_API UTowerDefenceGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "TowerDefenceGameInstance",
		meta = (ToolTip = "Test Load Asset"))
	void TestLoadAsset(FPrimaryAssetId AssetId);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TowerDefenceGameInstance")
	TObjectPtr<ULevelDataAsset> CurrentLevelData;

	UFUNCTION(BlueprintCallable, Category = "TowerDefenceGameInstance",
		meta = (ToolTip = "TestOnAssetLoaded"))
	void OnAssetLoaded(FPrimaryAssetId LoadedId);
};

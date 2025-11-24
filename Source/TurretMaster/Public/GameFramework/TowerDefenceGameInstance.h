#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TowerDefenceGameInstance.generated.h"

class ULevelDataAsset;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLevelDataLoaded, ULevelDataAsset*, LevelData);

/**
 * Game instance for the tower defence game
 * Primarily handles loading if level data for each level
 */
UCLASS()
class TURRETMASTER_API UTowerDefenceGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "TowerDefenceGameInstance")
	FOnLevelDataLoaded OnLevelDataLoaded;

	UFUNCTION(BlueprintCallable, Category = "TowerDefenceGameInstance",
		meta = (ToolTip = "Loads the appropriate level data, using a soft pointer to a level world"))
	void LoadDataUsingLevel(TSoftObjectPtr<UWorld> InWorld);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "TowerDefenceGameInstance",
		meta = (ToolTip = "Gets a world soft pointer. For use in blueprints"))
	UWorld* GetLevel();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TowerDefenceGameInstance")
	TMap<FString, FPrimaryAssetId> LevelDataIdMap;

	UPROPERTY(BlueprintReadWrite, Category = "TowerDefenceGameInstance")
	TObjectPtr<ULevelDataAsset> CurrentLevelData;

	UFUNCTION(BlueprintCallable, Category = "TowerDefenceGameInstance",
		meta = (ToolTip = "Sends an event when the level data asset has been loaded"))
	void OnAssetLoaded(FPrimaryAssetId LoadedId);
};

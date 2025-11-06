#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BuildingDataAsset.generated.h"

/**
 * Data asset for storing accessible information about each building type
 */
UCLASS()
class TURRETMASTER_API UBuildingDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BuildingDataAsset")
	FText Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BuildingDataAsset")
	FText Description;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BuildingDataAsset")
	TObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BuildingDataAsset")
	FColor Colour;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BuildingDataAsset")
	TObjectPtr<USkeletalMesh> SkeletalMesh;

	//TODO: Disucss, is there a better way of doing this so it requires IBuildable?

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BuildingDataAsset", meta = (MustImplement = "Buildable"))
	TSubclassOf<AActor> Class;
};

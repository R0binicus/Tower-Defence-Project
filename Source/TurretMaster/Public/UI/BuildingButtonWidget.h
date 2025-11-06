#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "DataAssets/BuildingDataAsset.h"
#include "BuildingButtonWidget.generated.h"

/**
 * Widget which displays a button for creating a new building
 * and includes other information about the building
 */
UCLASS()
class TURRETMASTER_API UBuildingButtonWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, Category = "BuildingButtonWidget")
	TObjectPtr<UBuildingDataAsset> BuildingDataAsset;

	// Bind Widget

	UPROPERTY(BlueprintReadOnly, Category = "BuildingButtonWidget", meta = (BindWidget))
	TObjectPtr<UImage> BuildingIcon;

	UPROPERTY(BlueprintReadOnly, Category = "BuildingButtonWidget", meta = (BindWidget))
	TObjectPtr<UTextBlock> CostTextBlock;
};

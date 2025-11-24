#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BuildingsWidget.generated.h"

class UBuildingButtonWidget;
class UBuildingDataAsset;
class UBuildingButtonWidget;
class UCanvasPanel;

/**
 * Widget parent, which contains a button widget for each building type
 */
UCLASS()
class TURRETMASTER_API UBuildingsWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BuildingsWidget")
	TSubclassOf<UBuildingButtonWidget> BuildingButtonWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BuildingsWidget")
	TArray<TObjectPtr<UBuildingDataAsset>> BuildableTurretDataArray;

	UPROPERTY(BlueprintReadWrite, Category = "BuildingsWidget")
	TArray<TObjectPtr<UBuildingButtonWidget>> BuildingWidgetArray;

	// Bind Widget

	UPROPERTY(BlueprintReadOnly, Category = "BuildingsWidget", meta = (BindWidget))
	TObjectPtr<UCanvasPanel> MainCanvasPanel;
};

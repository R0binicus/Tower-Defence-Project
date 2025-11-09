#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "UI/BuildingButtonWidget.h"
#include "DataAssets/BuildingDataAsset.h"
#include "BuildingsWidget.generated.h"

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

	void NativeConstruct() override;
};

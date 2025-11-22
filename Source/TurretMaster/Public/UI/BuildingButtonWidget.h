#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "DataAssets/BuildingDataAsset.h"
#include "Subsystems/BuildingSubsystem.h"
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BuildingButtonWidget")
	TObjectPtr<UBuildingDataAsset> BuildingDataAsset;

	// Bind Widget

	UPROPERTY(BlueprintReadOnly, Category = "BuildingButtonWidget", meta = (BindWidget))
	TObjectPtr<UButton> Button;

	UPROPERTY(BlueprintReadOnly, Category = "BuildingButtonWidget", meta = (BindWidget))
	TObjectPtr<UImage> BuildingIcon;

	UPROPERTY(BlueprintReadOnly, Category = "BuildingButtonWidget", meta = (BindWidget))
	TObjectPtr<UTextBlock> CostTextBlock;

	UPROPERTY()
	TObjectPtr<UBuildingSubsystem> BuildingSubsystem;

	void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, Category = "BuildingButtonWidget",
		meta = (ToolTip = "Triggered when the button is clicked"))
	void OnButtonClicked();

	UFUNCTION(BlueprintCallable, Category = "BuildingButtonWidget",
		meta = (ToolTip = "Triggered when the button is hovered over by the mouse, or is selected"))
	void OnButtonHovered();

	UFUNCTION(BlueprintCallable, Category = "BuildingButtonWidget",
		meta = (ToolTip = "Triggered when the button is de-selected"))
	void OnButtonUnhovered();
};

#include "UI/BuildingInfoDisplayWidget.h"
#include "Components/TextBlock.h"
#include "Subsystems/BuildingSubsystem.h"
#include "DataAssets/BuildingDataAsset.h"
#include "Turret.h"

void UBuildingInfoDisplayWidget::NativeConstruct()
{
	TObjectPtr<UBuildingSubsystem> BuildingSubsystem = GetWorld()->GetSubsystem<UBuildingSubsystem>();
	if (BuildingSubsystem)
	{
		BuildingSubsystem->OnBuildingHighlighted.AddUniqueDynamic(this, &UBuildingInfoDisplayWidget::UpdateBuildingInfoDisplay);
	}
}

void UBuildingInfoDisplayWidget::UpdateBuildingInfoDisplay(const UBuildingDataAsset* BuildingData, const ATurret* Turret)
{
	if (!BuildingData)
	{
		HideBuildingDisplay();
		return;
	}

	if (!BuildingName || !BuildingDesc)
	{
		return;
	}

	if (!Turret)
	{
		// Hide button
		return;
	}

	BuildingName->SetText(BuildingData->Name);
	BuildingDesc->SetText(BuildingData->Description);
}

void UBuildingInfoDisplayWidget::HideBuildingDisplay()
{
	if (!BuildingName || !BuildingDesc)
	{
		return;
	}

	BuildingName->SetText(FText());
	BuildingDesc->SetText(FText());
	// Hide button
}

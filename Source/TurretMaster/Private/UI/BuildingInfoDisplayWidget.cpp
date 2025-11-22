#include "UI/BuildingInfoDisplayWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
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

	if (SellButton)
	{
		SellButton->OnClicked.AddDynamic(this, &UBuildingInfoDisplayWidget::SellBuildingPressed);
	}

	HideBuildingDisplay();
}

void UBuildingInfoDisplayWidget::UpdateBuildingInfoDisplay(const UBuildingDataAsset* BuildingData, ATurret* Turret)
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

	SelectedTurret = Turret;
	if (SelectedTurret)
	{
		SellButton->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		SellButton->SetVisibility(ESlateVisibility::Hidden);
	}

	BuildingName->SetText(BuildingData->Name);
	BuildingDesc->SetText(BuildingData->Description);
}

void UBuildingInfoDisplayWidget::HideBuildingDisplay()
{
	SelectedTurret = nullptr;

	if (!BuildingName || !BuildingDesc)
	{
		return;
	}

	BuildingName->SetText(FText());
	BuildingDesc->SetText(FText());

	if (SellButton)
	{
		SellButton->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UBuildingInfoDisplayWidget::SellBuildingPressed()
{
	if (!SelectedTurret)
	{
		return;
	}

	SelectedTurret->Destroy();
	HideBuildingDisplay();
}
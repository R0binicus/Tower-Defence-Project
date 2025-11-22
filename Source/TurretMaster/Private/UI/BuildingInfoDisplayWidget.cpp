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

void UBuildingInfoDisplayWidget::UpdateBuildingInfoDisplay(UBuildingDataAsset* BuildingData, ATurret* Turret)
{
	SelectedTurretData = BuildingData;
	if (!SelectedTurretData)
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

	BuildingName->SetText(SelectedTurretData->Name);
	BuildingDesc->SetText(SelectedTurretData->Description);
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
	if (!SelectedTurret || !SelectedTurretData)
	{
		return;
	}

	SelectedTurretData->Cost;

	TObjectPtr<ATowerDefencePlayerState> PlayerStateClass = Cast<ATowerDefencePlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0));
	if (!PlayerStateClass)
	{
		return;
	}

	PlayerStateClass->ChangeCurrentMoney(SelectedTurretData->Cost * SellFraction);

	SelectedTurret->Destroy();
	HideBuildingDisplay();
}
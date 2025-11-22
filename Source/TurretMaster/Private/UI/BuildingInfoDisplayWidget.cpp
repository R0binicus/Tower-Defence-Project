#include "UI/BuildingInfoDisplayWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Subsystems/BuildingSubsystem.h"
#include "DataAssets/BuildingDataAsset.h"

void UBuildingInfoDisplayWidget::NativeConstruct()
{
	TObjectPtr<UBuildingSubsystem> BuildingSubsystem = GetWorld()->GetSubsystem<UBuildingSubsystem>();
	if (BuildingSubsystem)
	{
		BuildingSubsystem->OnBuildingHighlighted.AddUniqueDynamic(this, &UBuildingInfoDisplayWidget::UpdateBuildingInfoDisplay);
	}

	PlayerState = Cast<ATowerDefencePlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0));

	if (SellButton)
	{
		SellButton->OnClicked.AddDynamic(this, &UBuildingInfoDisplayWidget::SellBuildingPressed);
	}

	HideBuildingDisplay();
}

void UBuildingInfoDisplayWidget::UpdateBuildingInfoDisplay(UBuildingDataAsset* BuildingData, ATurret* Turret)
{
	if (!BuildingData)
	{
		HideBuildingDisplay();
		return;
	}

	if (!BuildingName || !BuildingDesc || !SellButtonText || !PlayerState)
	{
		return;
	}

	if (Turret)
	{
		int32 SellReturnAmount = BuildingData->Cost * PlayerState->GetSellReturnFraction();
		FString FormattedNum = FString::Printf(TEXT("%s%i"), *SellTextPrefix, SellReturnAmount);
		SellButtonText->SetText(FText::FromString(FormattedNum));

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
	if (!PlayerState)
	{
		return;
	}

	PlayerState->SellBuilding();
	HideBuildingDisplay();
}
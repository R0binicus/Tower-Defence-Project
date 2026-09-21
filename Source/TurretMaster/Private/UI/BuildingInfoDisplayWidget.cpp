#include "UI/BuildingInfoDisplayWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Subsystems/BuildingSubsystem.h"
#include "DataAssets/BuildingDataAsset.h"
#include "GameFramework/TowerDefencePlayerState.h"
#include "Kismet/GameplayStatics.h"

void UBuildingInfoDisplayWidget::NativeConstruct()
{
	const TObjectPtr<UBuildingSubsystem> BuildingSubsystem = GetWorld()->GetSubsystem<UBuildingSubsystem>();
	if (IsValid(BuildingSubsystem))
	{
		BuildingSubsystem->OnBuildingHighlighted.AddUniqueDynamic(this, &UBuildingInfoDisplayWidget::UpdateBuildingInfoDisplay);
	}

	PlayerState = Cast<ATowerDefencePlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0));

	if (IsValid(SellButton))
	{
		SellButton->OnClicked.AddDynamic(this, &UBuildingInfoDisplayWidget::SellBuildingPressed);
	}

	HideBuildingDisplay();
}

void UBuildingInfoDisplayWidget::UpdateBuildingInfoDisplay(UBuildingDataAsset* BuildingData, ATurret* Turret)
{
	if (!IsValid(BuildingData))
	{
		HideBuildingDisplay();
		return;
	}

	if (!IsValid(BuildingName) || !IsValid(BuildingDesc) || 
		!IsValid(SellButtonText) || !IsValid(PlayerState))
	{
		return;
	}

	if (Turret)
	{
		const int32 SellReturnAmount = BuildingData->Cost * PlayerState->GetSellReturnFraction();
		const FString FormattedNum = FString::Printf(TEXT("%s%i"), *SellTextPrefix, SellReturnAmount);
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

void UBuildingInfoDisplayWidget::HideBuildingDisplay() const
{
	if (IsValid(BuildingName) && IsValid(BuildingDesc) && IsValid(SellButton))
	{
		BuildingName->SetText(FText());
		BuildingDesc->SetText(FText());
		SellButton->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UBuildingInfoDisplayWidget::SellBuildingPressed()
{
	if (IsValid(PlayerState))
	{
		PlayerState->SellBuilding();
		HideBuildingDisplay();
	}
}

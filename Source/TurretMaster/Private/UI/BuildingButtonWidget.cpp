#include "UI/BuildingButtonWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "DataAssets/BuildingDataAsset.h"
#include "Subsystems/BuildingSubsystem.h"

void UBuildingButtonWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (!Button || !BuildingIcon || !BuildingDataAsset || !CostTextBlock)
    {
        return;
    }

    Button->OnClicked.AddDynamic(this, &UBuildingButtonWidget::OnButtonClicked);
    Button->OnHovered.AddDynamic(this, &UBuildingButtonWidget::OnButtonHovered);
    Button->OnUnhovered.AddDynamic(this, &UBuildingButtonWidget::OnButtonUnhovered);

    BuildingIcon->SetBrushFromTexture(BuildingDataAsset->Icon);

    const FString FormattedNum = FString::Printf(TEXT("$%i"), BuildingDataAsset->Cost);
    CostTextBlock->SetText(FText::FromString(FormattedNum));

    BuildingSubsystem = GetWorld()->GetSubsystem<UBuildingSubsystem>();
}

void UBuildingButtonWidget::OnButtonClicked()
{
    if (BuildingSubsystem)
    {
        BuildingSubsystem->SelectedPlaceBuilding(BuildingDataAsset);
    }
}

void UBuildingButtonWidget::OnButtonHovered()
{
    if (BuildingSubsystem)
    {
        BuildingSubsystem->OnBuildingHighlighted.Broadcast(BuildingDataAsset, nullptr);
    }
}

void UBuildingButtonWidget::OnButtonUnhovered()
{
    if (BuildingSubsystem)
    {
        BuildingSubsystem->OnBuildingHighlighted.Broadcast(nullptr, nullptr);
    }
}

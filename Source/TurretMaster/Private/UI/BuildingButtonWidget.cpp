#include "UI/BuildingButtonWidget.h"

void UBuildingButtonWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (Button)
    {
        Button->OnClicked.AddDynamic(this, &UBuildingButtonWidget::OnButtonClicked);
        Button->OnHovered.AddDynamic(this, &UBuildingButtonWidget::OnButtonHovered);
        Button->OnUnhovered.AddDynamic(this, &UBuildingButtonWidget::OnButtonUnhovered);
    }
    
    if (!BuildingDataAsset)
    {
        return;
    }

    if (BuildingIcon)
    {
        BuildingIcon->SetBrushFromTexture(BuildingDataAsset->Icon);
    }

    if (CostTextBlock)
    {
        FString FormattedNum = FString::Printf(TEXT("$%i"), BuildingDataAsset->Cost);
        CostTextBlock->SetText(FText::FromString(FormattedNum));
    }

    BuildingSubsystem = GetWorld()->GetSubsystem<UBuildingSubsystem>();
}

void UBuildingButtonWidget::OnButtonClicked()
{
    if (!BuildingSubsystem)
    {
        return;
    }

    BuildingSubsystem->SelectedPlaceBuilding(BuildingDataAsset);
}

void UBuildingButtonWidget::OnButtonHovered()
{
    if (!BuildingSubsystem)
    {
        return;
    }

    BuildingSubsystem->OnBuildingHighlighted.Broadcast(BuildingDataAsset, nullptr);
}

void UBuildingButtonWidget::OnButtonUnhovered()
{
    if (!BuildingSubsystem)
    {
        return;
    }

    BuildingSubsystem->OnBuildingHighlighted.Broadcast(nullptr, nullptr);
}

#include "UI/BuildingButtonWidget.h"

void UBuildingButtonWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (Button)
    {
        Button->OnClicked.AddDynamic(this, &UBuildingButtonWidget::OnButtonClicked);
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
}

void UBuildingButtonWidget::OnButtonClicked()
{
    TObjectPtr<UBuildingSubsystem> BuildingSubsystem = GetWorld()->GetSubsystem<UBuildingSubsystem>();
    if (!BuildingSubsystem)
    {
        return;
    }

    BuildingSubsystem->SelectedPlaceBuilding(BuildingDataAsset);
}

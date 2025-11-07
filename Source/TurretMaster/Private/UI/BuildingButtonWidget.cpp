#include "UI/BuildingButtonWidget.h"

void UBuildingButtonWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (!Button)
    {
        return;
    }

    Button->OnClicked.AddDynamic(this, &UBuildingButtonWidget::OnButtonClicked);
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

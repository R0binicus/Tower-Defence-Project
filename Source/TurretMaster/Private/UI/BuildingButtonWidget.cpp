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
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("OnButtonClicked")));
}

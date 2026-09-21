#include "UI/BuildingButtonWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "DataAssets/BuildingDataAsset.h"
#include "Subsystems/BuildingSubsystem.h"
#include "GameFramework/TowerDefencePlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Framework/Application/SlateApplication.h"

void UBuildingButtonWidget::NativeConstruct()
{
    Super::NativeConstruct();
    if (!IsValid(Button) || !IsValid(BuildingIcon) || !IsValid(BuildingDataAsset) || !IsValid(CostTextBlock))
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

    const TObjectPtr<ATowerDefencePlayerController> PlayerController = Cast<ATowerDefencePlayerController>(UGameplayStatics::GetPlayerController(this, 0));
    if (PlayerController)
    {
        PlayerController->OnSelectInput.AddUniqueDynamic(this, &UBuildingButtonWidget::OnButtonClicked);
    }
}

void UBuildingButtonWidget::OnButtonClicked()
{
    if (IsValid(BuildingSubsystem) && IsHovered())
    {
        BuildingSubsystem->SelectedPlaceBuilding(BuildingDataAsset);
    }
}

void UBuildingButtonWidget::OnButtonHovered()
{
    if (IsValid(BuildingSubsystem))
    {
        BuildingSubsystem->OnBuildingHighlighted.Broadcast(BuildingDataAsset, nullptr);
    }
}

void UBuildingButtonWidget::OnButtonUnhovered()
{
    if (IsValid(BuildingSubsystem))
    {
        BuildingSubsystem->OnBuildingHighlighted.Broadcast(nullptr, nullptr);
    }
}

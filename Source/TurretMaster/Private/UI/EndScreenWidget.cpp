#include "UI/EndScreenWidget.h"

void UEndScreenWidget::NativeConstruct()
{
	if (RestartButton)
	{
		RestartButton->OnClicked.AddDynamic(this, &UEndScreenWidget::OnRestartClicked);
	}

	if (MainMenuButton)
	{
		MainMenuButton->OnClicked.AddDynamic(this, &UEndScreenWidget::OnMainMenuClicked);
	}
}

void UEndScreenWidget::ShowMenu()
{
	SetVisibility(ESlateVisibility::Visible);
}

void UEndScreenWidget::HideMenu()
{
	SetVisibility(ESlateVisibility::Hidden);
}

void UEndScreenWidget::OnRestartClicked()
{
	FString CurrentLevelNameString = UGameplayStatics::GetCurrentLevelName(GetWorld());
	UGameplayStatics::OpenLevel(this, FName(CurrentLevelNameString));
}

void UEndScreenWidget::OnMainMenuClicked()
{
	if (!MainMenuLevel.IsNull())
	{
		UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), MainMenuLevel);
	}
}

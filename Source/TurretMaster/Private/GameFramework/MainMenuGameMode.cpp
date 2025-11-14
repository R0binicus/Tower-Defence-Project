#include "GameFramework/MainMenuGameMode.h"

void AMainMenuGameMode::BeginPlay()
{
	if (!MainMenuWidgetClass)
	{
		return;
	}

	MainMenuWidget = CreateWidget<UMainMenuWidget>(GetWorld(), MainMenuWidgetClass);
	if (!MainMenuWidget)
	{
		return;
	}

	MainMenuWidget->AddToPlayerScreen();
	MainMenuWidget->SetVisibility(ESlateVisibility::Visible);
}
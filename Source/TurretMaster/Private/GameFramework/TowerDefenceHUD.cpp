#include "GameFramework/TowerDefenceHUD.h"

void ATowerDefenceHUD::BeginPlay()
{
	if (!HudWidgetClass)
	{
		return;
	}

	if (!VictoryWidgetClass)
	{
		return;
	}

	if (!DefeatWidgetClass)
	{
		return;
	}

	HudWidget = CreateWidget<UHudWidget>(GetWorld(), HudWidgetClass);
	if (!HudWidget)
	{
		return;
	}

	VictoryWidget = CreateWidget<UEndScreenWidget>(GetWorld(), VictoryWidgetClass);
	if (!VictoryWidget)
	{
		return;
	}

	DefeatWidget = CreateWidget<UEndScreenWidget>(GetWorld(), DefeatWidgetClass);
	if (!DefeatWidget)
	{
		return;
	}

	HudWidget->AddToPlayerScreen();
	VictoryWidget->AddToPlayerScreen();
	DefeatWidget->AddToPlayerScreen();

	VictoryWidget->HideMenu();
	DefeatWidget->HideMenu();
}

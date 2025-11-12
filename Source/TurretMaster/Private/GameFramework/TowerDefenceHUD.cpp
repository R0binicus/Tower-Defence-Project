#include "GameFramework/TowerDefenceHUD.h"
#include "UI/HudWidget.h"
#include "UI/EndScreenWidget.h"

void ATowerDefenceHUD::SetVictoryWidgetVisible(bool bIsVisible)
{
	if (VictoryWidget)
	{
		VictoryWidget->SetWidgetVisible(bIsVisible);
	}
}

void ATowerDefenceHUD::SetDefeatWidgetVisible(bool bIsVisible)
{
	if (DefeatWidget)
	{
		DefeatWidget->SetWidgetVisible(bIsVisible);
	}
}

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

	VictoryWidget->SetWidgetVisible(false);
	DefeatWidget->SetWidgetVisible(false);
}

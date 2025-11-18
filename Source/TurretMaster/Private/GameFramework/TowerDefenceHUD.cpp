#include "GameFramework/TowerDefenceHUD.h"
#include "UI/PauseMenuWidget.h"
#include "UI/HudWidget.h"
#include "UI/EndScreenWidget.h"

void ATowerDefenceHUD::SetPauseWidgetVisible(bool bIsVisible)
{
	if (PauseWidget)
	{
		PauseWidget->SetWidgetVisible(bIsVisible);
	}
}

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

	if (!PauseWidgetClass)
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

	PauseWidget = CreateWidget<UPauseMenuWidget>(GetWorld(), PauseWidgetClass);
	if (!PauseWidget)
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
	PauseWidget->AddToPlayerScreen();
	VictoryWidget->AddToPlayerScreen();
	DefeatWidget->AddToPlayerScreen();

	PauseWidget->SetWidgetVisible(false);
	VictoryWidget->SetWidgetVisible(false);
	DefeatWidget->SetWidgetVisible(false);
}

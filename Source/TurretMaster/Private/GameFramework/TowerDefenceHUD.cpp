#include "GameFramework/TowerDefenceHUD.h"
#include "UI/PauseMenuWidget.h"
#include "UI/HudWidget.h"
#include "UI/EndScreenWidget.h"

void ATowerDefenceHUD::SetPauseWidgetVisible(const bool bIsVisible) const
{
	if (PauseWidget)
	{
		PauseWidget->SetWidgetVisible(bIsVisible);
	}
}

void ATowerDefenceHUD::SetVictoryWidgetVisible(const bool bIsVisible) const
{
	if (VictoryWidget)
	{
		VictoryWidget->SetWidgetVisible(bIsVisible);
	}
}

void ATowerDefenceHUD::SetDefeatWidgetVisible(const bool bIsVisible) const
{
	if (DefeatWidget)
	{
		DefeatWidget->SetWidgetVisible(bIsVisible);
	}
}

void ATowerDefenceHUD::BeginPlay()
{
	if (!HudWidgetClass || !PauseWidgetClass || !VictoryWidgetClass || !DefeatWidgetClass)
	{
		return;
	}

	PauseWidget = CreateWidget<UPauseMenuWidget>(GetWorld(), PauseWidgetClass);
	HudWidget = CreateWidget<UHudWidget>(GetWorld(), HudWidgetClass);
	VictoryWidget = CreateWidget<UEndScreenWidget>(GetWorld(), VictoryWidgetClass);
	DefeatWidget = CreateWidget<UEndScreenWidget>(GetWorld(), DefeatWidgetClass);
	
	if (!PauseWidget || !HudWidget || !VictoryWidget || !DefeatWidget)
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

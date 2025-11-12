#include "GameFramework/TowerDefenceHUD.h"

void ATowerDefenceHUD::BeginPlay()
{
	if (!PlayerHudClass)
	{
		return;
	}

	HudWidget = CreateWidget<UHudWidget>(GetWorld(), PlayerHudClass);
	if (!HudWidget)
	{
		return;
	}

	HudWidget->AddToPlayerScreen();
}

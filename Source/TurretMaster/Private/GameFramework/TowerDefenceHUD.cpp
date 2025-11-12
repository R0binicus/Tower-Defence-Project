#include "GameFramework/TowerDefenceHUD.h"

void ATowerDefenceHUD::BeginPlay()
{
	if (!PlayerHudClass)
	{
		return;
	}

	PlayerHUD = CreateWidget<UPlayerHUD>(GetWorld(), PlayerHudClass);
	if (!PlayerHUD)
	{
		return;
	}

	PlayerHUD->AddToPlayerScreen();
}

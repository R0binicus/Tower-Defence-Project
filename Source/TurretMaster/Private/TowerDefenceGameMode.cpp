#include "TowerDefenceGameMode.h"

void ATowerDefenceGameMode::BeginPlay()
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

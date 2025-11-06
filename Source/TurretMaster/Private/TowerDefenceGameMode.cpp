#include "TowerDefenceGameMode.h"

void ATowerDefenceGameMode::BeginPlay()
{
	PlayerHUD = CreateWidget<UPlayerHUD>(GetWorld(), PlayerHudClass);
	if (!PlayerHUD)
	{
		return;
	}

	PlayerHUD->AddToPlayerScreen();
}

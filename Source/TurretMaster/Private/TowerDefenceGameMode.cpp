#include "TowerDefenceGameMode.h"

ATowerDefenceGameMode::ATowerDefenceGameMode()
{
	PlayerStateClass = ATowerDefencePlayerState::StaticClass();
}

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

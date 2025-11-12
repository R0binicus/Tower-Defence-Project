#include "TowerDefenceGameMode.h"

ATowerDefenceGameMode::ATowerDefenceGameMode()
{
	PlayerStateClass = ATowerDefencePlayerState::StaticClass();
	GameStateClass = ATowerDefenceGameState::StaticClass();
	HUDClass = ATowerDefenceHUD::StaticClass();
}

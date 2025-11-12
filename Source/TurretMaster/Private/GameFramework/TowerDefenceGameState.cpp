#include "GameFramework/TowerDefenceGameState.h"

void ATowerDefenceGameState::TriggerWin()
{
	OnGameWin.Broadcast();

	TObjectPtr<ATowerDefenceHUD> GameHUD = Cast<ATowerDefenceHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	if (GameHUD)
	{
		GameHUD->SetVictoryWidgetVisible(true);
	}
}

void ATowerDefenceGameState::TwiggerLose()
{
	if (bGameLost)
	{
		return;
	}

	bGameLost = true;
	OnGameLose.Broadcast();

	TObjectPtr<ATowerDefenceHUD> GameHUD = Cast<ATowerDefenceHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	if (GameHUD)
	{
		GameHUD->SetDefeatWidgetVisible(true);
	}
}

#include "GameFramework/TowerDefenceGameState.h"

void ATowerDefenceGameState::TriggerWin()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("You Win!!! :)")));
	OnGameWin.Broadcast();
}

void ATowerDefenceGameState::TwiggerLose()
{
	if (bGameLost)
	{
		return;
	}

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("You Lose!!! :(")));
	bGameLost = true;
	OnGameLose.Broadcast();
}

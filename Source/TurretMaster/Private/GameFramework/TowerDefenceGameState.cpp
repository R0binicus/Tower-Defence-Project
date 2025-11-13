#include "GameFramework/TowerDefenceGameState.h"

void ATowerDefenceGameState::BeginPlay()
{
	Super::BeginPlay();

	TObjectPtr<ATowerDefencePlayerController> PlayerController = Cast<ATowerDefencePlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (PlayerController)
	{
		PlayerController->OnPauseInput.AddDynamic(this, &ATowerDefenceGameState::OnPauseInputEvent);
	}
}

void ATowerDefenceGameState::TriggerWin()
{
	OnGameWin.Broadcast();

	TObjectPtr<ATowerDefenceHUD> GameHUD = Cast<ATowerDefenceHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	if (GameHUD)
	{
		GameHUD->SetVictoryWidgetVisible(true);
	}
}

void ATowerDefenceGameState::TriggerLose()
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

void ATowerDefenceGameState::SetGamePaused(bool bIsNowPaused)
{
	UGameplayStatics::SetGamePaused(GetWorld(), bIsNowPaused);
	OnGamePaused.Broadcast(bIsNowPaused);
}

void ATowerDefenceGameState::OnPauseInputEvent()
{
	bool bIsPaused = UGameplayStatics::IsGamePaused(GetWorld());
	SetGamePaused(!bIsPaused);
}

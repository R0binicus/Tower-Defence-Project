#include "GameFramework/TowerDefenceGameState.h"
#include "GameFramework/TowerDefenceHUD.h"
#include "GameFramework/TowerDefencePlayerController.h"
#include "Kismet/GameplayStatics.h"

void ATowerDefenceGameState::BeginPlay()
{
	Super::BeginPlay();

	const TObjectPtr<ATowerDefencePlayerController> PlayerController = Cast<ATowerDefencePlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (PlayerController)
	{
		PlayerController->OnPauseInput.AddDynamic(this, &ATowerDefenceGameState::OnPauseInputEvent);
	}
}

void ATowerDefenceGameState::TriggerWin() const
{
	OnGameWin.Broadcast();

	const TObjectPtr<ATowerDefenceHUD> GameHUD = Cast<ATowerDefenceHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
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

	const TObjectPtr<ATowerDefenceHUD> GameHUD = Cast<ATowerDefenceHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	if (GameHUD)
	{
		GameHUD->SetDefeatWidgetVisible(true);
	}
}

void ATowerDefenceGameState::SetGamePaused(const bool bIsNowPaused) const
{
	UGameplayStatics::SetGamePaused(GetWorld(), bIsNowPaused);
	OnGamePaused.Broadcast(bIsNowPaused);
}

void ATowerDefenceGameState::OnPauseInputEvent()
{
	const bool bIsPaused = UGameplayStatics::IsGamePaused(GetWorld());
	SetGamePaused(!bIsPaused);
}

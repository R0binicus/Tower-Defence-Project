#include "UI/PauseMenuWidget.h"
#include "Components/Button.h"
#include "GameFramework/TowerDefenceGameState.h"
#include "Kismet/GameplayStatics.h"

void UPauseMenuWidget::NativeConstruct()
{
	if (ResumeButton)
	{
		ResumeButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnResumeClicked);
	}

	if (RestartButton)
	{
		RestartButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnRestartClicked);
	}

	if (MainMenuButton)
	{
		MainMenuButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnMainMenuClicked);
	}

	TObjectPtr<ATowerDefenceGameState> GameState = Cast<ATowerDefenceGameState>(GetWorld()->GetGameState());
	if (GameState)
	{
		GameState->OnGamePaused.AddUniqueDynamic(this, &UPauseMenuWidget::SetWidgetVisible);
	}
}

void UPauseMenuWidget::SetWidgetVisible(bool bIsVisible)
{
	if (bIsVisible)
	{
		SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		SetVisibility(ESlateVisibility::Hidden);
	}
}

void UPauseMenuWidget::SetWidgetPaused(bool bIsNowPaused)
{
	TObjectPtr<ATowerDefenceGameState> GameState = Cast<ATowerDefenceGameState>(GetWorld()->GetGameState());
	if (GameState)
	{
		GameState->SetGamePaused(bIsNowPaused);
	}
}

void UPauseMenuWidget::OnResumeClicked()
{
	SetWidgetPaused(false);
}

void UPauseMenuWidget::OnRestartClicked()
{
	FString CurrentLevelNameString = UGameplayStatics::GetCurrentLevelName(GetWorld());
	UGameplayStatics::OpenLevel(this, FName(CurrentLevelNameString));
}

void UPauseMenuWidget::OnMainMenuClicked()
{
	if (MainMenuLevel.IsValid() || MainMenuLevel.IsPending())
	{
		UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), MainMenuLevel);
	}
}


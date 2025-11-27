#include "UI/PauseMenuWidget.h"
#include "Components/Button.h"
#include "GameFramework/TowerDefenceGameState.h"
#include "Kismet/GameplayStatics.h"

void UPauseMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!ResumeButton || !RestartButton || !MainMenuButton)
	{
		return;
	}
	ResumeButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnResumeClicked);
	MainMenuButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnMainMenuClicked);
	RestartButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnRestartClicked);

	DefaultButton = ResumeButton;

	const TObjectPtr<ATowerDefenceGameState> GameState = Cast<ATowerDefenceGameState>(GetWorld()->GetGameState());
	if (GameState)
	{
		GameState->OnGamePaused.AddUniqueDynamic(this, &UPauseMenuWidget::SetWidgetVisible);
	}
}

void UPauseMenuWidget::SetWidgetPaused(const bool bIsNowPaused) const
{
	FSlateApplication::Get().SetUserFocusToGameViewport(0, EFocusCause::Cleared);
	const TObjectPtr<ATowerDefenceGameState> GameState = Cast<ATowerDefenceGameState>(GetWorld()->GetGameState());
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
	const FString CurrentLevelNameString = UGameplayStatics::GetCurrentLevelName(GetWorld());
	UGameplayStatics::OpenLevel(this, FName(CurrentLevelNameString));
}

void UPauseMenuWidget::OnMainMenuClicked()
{
	if (MainMenuLevel.IsValid() || MainMenuLevel.IsPending())
	{
		UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), MainMenuLevel);
	}
}


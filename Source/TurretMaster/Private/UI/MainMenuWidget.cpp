#include "UI/MainMenuWidget.h"

void UMainMenuWidget::NativeConstruct()
{
	if (PlayButton)
	{
		PlayButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnPlayClicked);
	}

	if (QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnQuitClicked);
	}
}

void UMainMenuWidget::OnPlayClicked()
{
	if (!FirstLevel.IsNull())
	{
		UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), FirstLevel);
	}
}

void UMainMenuWidget::OnQuitClicked()
{
	UKismetSystemLibrary::QuitGame(this, UGameplayStatics::GetPlayerController(GetWorld(), 0), EQuitPreference::Quit, false);
}
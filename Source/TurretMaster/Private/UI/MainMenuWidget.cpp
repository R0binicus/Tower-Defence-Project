#include "UI/MainMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UMainMenuWidget::NativeConstruct()
{
	if (Level01Button)
	{
		Level01Button->OnClicked.AddDynamic(this, &UMainMenuWidget::OnLevel01Clicked);
	}

	if (QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnQuitClicked);
	}
}

void UMainMenuWidget::OnLevel01Clicked()
{
	if (Level01.IsNull())
	{
		return;
	}

	UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), Level01);
}

void UMainMenuWidget::OnQuitClicked()
{
	UKismetSystemLibrary::QuitGame(this, UGameplayStatics::GetPlayerController(GetWorld(), 0), EQuitPreference::Quit, false);
}
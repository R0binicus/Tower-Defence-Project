#include "UI/MainMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!Level01Button || !QuitButton)
	{
		return;
	}

	Level01Button->OnClicked.AddDynamic(this, &UMainMenuWidget::OnLevel01Clicked);
	QuitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnQuitClicked);

	DefaultButton = Level01Button;
	FocusDefaultButton();
}

void UMainMenuWidget::OnLevel01Clicked()
{
	FSlateApplication::Get().SetUserFocusToGameViewport(0, EFocusCause::Cleared);
	if (!Level01.IsNull())
	{
		UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), Level01);
	}
}

void UMainMenuWidget::OnQuitClicked()
{
	UKismetSystemLibrary::QuitGame(this, UGameplayStatics::GetPlayerController(GetWorld(), 0), EQuitPreference::Quit, false);
}
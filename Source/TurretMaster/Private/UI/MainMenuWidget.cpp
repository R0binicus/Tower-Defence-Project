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

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController)
	{
		PlayerController->SetInputMode(FInputModeGameAndUI());
	}

	DefaultButton = Level01Button;
	FocusDefaultButton();
}

void UMainMenuWidget::OnLevel01Clicked()
{
	if (!Level01.IsNull())
	{
		UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), Level01);
	}
}

void UMainMenuWidget::OnQuitClicked()
{
	UKismetSystemLibrary::QuitGame(this, UGameplayStatics::GetPlayerController(GetWorld(), 0), EQuitPreference::Quit, false);
}
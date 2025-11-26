#include "UI/EndScreenWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UEndScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!RestartButton || !MainMenuButton)
	{
		return;
	}

	RestartButton->OnClicked.AddDynamic(this, &UEndScreenWidget::OnRestartClicked);
	MainMenuButton->OnClicked.AddDynamic(this, &UEndScreenWidget::OnMainMenuClicked);
}

void UEndScreenWidget::OnRestartClicked()
{
	const FString CurrentLevelNameString = UGameplayStatics::GetCurrentLevelName(GetWorld());
	UGameplayStatics::OpenLevel(this, FName(CurrentLevelNameString));
}

void UEndScreenWidget::OnMainMenuClicked()
{
	if (MainMenuLevel.IsValid() || MainMenuLevel.IsPending())
	{
		UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), MainMenuLevel);
	}
}

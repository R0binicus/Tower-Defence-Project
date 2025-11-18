#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "GameFramework/TowerDefenceGameState.h"
#include "Kismet/GameplayStatics.h"
#include "PauseMenuWidget.generated.h"

/**
 * Widget that appears when the player pauses the game
 */
UCLASS()
class TURRETMASTER_API UPauseMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "PauseMenuWidget",
		meta = (ToolTip = "Makes the pause menu widget to be visible, or not visible"))
	void SetWidgetVisible(bool bIsVisible);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PauseMenuWidget")
	const TSoftObjectPtr<UWorld> MainMenuLevel;

	UPROPERTY(BlueprintReadOnly, Category = "PauseMenuWidget", meta = (BindWidget))
	TObjectPtr<UButton> ResumeButton;

	UPROPERTY(BlueprintReadOnly, Category = "PauseMenuWidget", meta = (BindWidget))
	TObjectPtr<UButton> RestartButton;

	UPROPERTY(BlueprintReadOnly, Category = "PauseMenuWidget", meta = (BindWidget))
	TObjectPtr<UButton> MainMenuButton;

	void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, Category = "PauseMenuWidget",
		meta = (ToolTip = "Tells the game state to pause the game"))
	void SetWidgetPaused(bool bIsPause);

	UFUNCTION(BlueprintCallable, Category = "PauseMenuWidget",
		meta = (ToolTip = "Starts the first level"))
	void OnResumeClicked();

	UFUNCTION(BlueprintCallable, Category = "PauseMenuWidget",
		meta = (ToolTip = "Restarts the current level"))
	void OnRestartClicked();

	UFUNCTION(BlueprintCallable, Category = "PauseMenuWidget",
		meta = (ToolTip = "Quits the game application"))
	void OnMainMenuClicked();
};

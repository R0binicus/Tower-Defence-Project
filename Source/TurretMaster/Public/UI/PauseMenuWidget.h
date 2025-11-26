#pragma once

#include "CoreMinimal.h"
#include "UI/MenuBaseWidget.h"
#include "PauseMenuWidget.generated.h"

class UButton;

/**
 * Widget that appears when the player pauses the game
 */
UCLASS()
class TURRETMASTER_API UPauseMenuWidget : public UMenuBaseWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PauseMenuWidget")
	const TSoftObjectPtr<UWorld> MainMenuLevel;

	UPROPERTY(BlueprintReadOnly, Category = "PauseMenuWidget", meta = (BindWidget))
	TObjectPtr<UButton> ResumeButton;

	UPROPERTY(BlueprintReadOnly, Category = "PauseMenuWidget", meta = (BindWidget))
	TObjectPtr<UButton> RestartButton;

	UPROPERTY(BlueprintReadOnly, Category = "PauseMenuWidget", meta = (BindWidget))
	TObjectPtr<UButton> MainMenuButton;

	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, Category = "PauseMenuWidget",
		meta = (ToolTip = "Tells the game state to pause the game"))
	void SetWidgetPaused(bool bIsPause) const;

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

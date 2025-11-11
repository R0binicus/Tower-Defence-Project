#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "EndScreenWidget.generated.h"

/**
 * Widget that appears when the player reaches an end game state
 * such as victory or defeat
 */
UCLASS()
class TURRETMASTER_API UEndScreenWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "EndScreenWidget", meta = (BindWidget))
	TObjectPtr<UButton> RestartButton;

	UPROPERTY(BlueprintReadOnly, Category = "EndScreenWidget", meta = (BindWidget))
	TObjectPtr<UButton> MainMenuButton;

	void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, Category = "EndScreenWidget",
		meta = (ToolTip = "Reveals the menu to the player"))
	void ShowMenu();

	UFUNCTION(BlueprintCallable, Category = "EndScreenWidget",
		meta = (ToolTip = "Hides the menu from the player"))
	void HideMenu();

	UFUNCTION(BlueprintCallable, Category = "EndScreenWidget",
		meta = (ToolTip = "Restarts the current level"))
	void OnRestartClicked();

	UFUNCTION(BlueprintCallable, Category = "EndScreenWidget",
		meta = (ToolTip = "Transitions the game back to the main menu"))
	void OnMainMenuClicked();
};

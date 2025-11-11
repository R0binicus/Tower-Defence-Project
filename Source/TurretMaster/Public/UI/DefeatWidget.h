#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "DefeatWidget.generated.h"

/**
 * Widget that appears when the player loses the game
 */
UCLASS()
class TURRETMASTER_API UDefeatWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "DefeatWidget", meta = (BindWidget))
	TObjectPtr<UButton> RestartButton;

	UPROPERTY(BlueprintReadOnly, Category = "DefeatWidget", meta = (BindWidget))
	TObjectPtr<UButton> MainMenuButton;

	void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, Category = "DefeatWidget",
		meta = (ToolTip = "Reveals the menu to the player"))
	void ShowMenu();

	UFUNCTION(BlueprintCallable, Category = "DefeatWidget",
		meta = (ToolTip = "Hides the menu from the player"))
	void HideMenu();

	UFUNCTION(BlueprintCallable, Category = "DefeatWidget",
		meta = (ToolTip = "Restarts the current level"))
	void OnRestartClicked();

	UFUNCTION(BlueprintCallable, Category = "DefeatWidget",
		meta = (ToolTip = "Transitions the game back to the main menu"))
	void OnMainMenuClicked();
};

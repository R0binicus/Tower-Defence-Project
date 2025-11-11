#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "VictoryWiget.generated.h"

/**
 * Widget that appears when the player wins the game
 */
UCLASS()
class TURRETMASTER_API UVictoryWiget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "VictoryWidget", meta = (BindWidget))
	TObjectPtr<UButton> RestartButton;

	UPROPERTY(BlueprintReadOnly, Category = "VictoryWidget", meta = (BindWidget))
	TObjectPtr<UButton> MainMenuButton;

	void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, Category = "VictoryWidget",
		meta = (ToolTip = "Reveals the menu to the player"))
	void ShowMenu();

	UFUNCTION(BlueprintCallable, Category = "VictoryWidget",
		meta = (ToolTip = "Hides the menu from the player"))
	void HideMenu();

	UFUNCTION(BlueprintCallable, Category = "VictoryWidget",
		meta = (ToolTip = "Restarts the current level"))
	void OnRestartClicked();

	UFUNCTION(BlueprintCallable, Category = "VictoryWidget",
		meta = (ToolTip = "Transitions the game back to the main menu"))
	void OnMainMenuClicked();
};

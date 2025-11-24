#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EndScreenWidget.generated.h"

class UButton;

/**
 * Widget that appears when the player reaches an end game state
 * such as victory or defeat
 */
UCLASS()
class TURRETMASTER_API UEndScreenWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "EndScreenWidget",
		meta = (ToolTip = "Makes the end screen widget to be visible, or not visible"))
	void SetWidgetVisible(bool bIsVisible);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EndScreenWidget")
	const TSoftObjectPtr<UWorld> MainMenuLevel;

	UPROPERTY(BlueprintReadOnly, Category = "EndScreenWidget", meta = (BindWidget))
	TObjectPtr<UButton> RestartButton;

	UPROPERTY(BlueprintReadOnly, Category = "EndScreenWidget", meta = (BindWidget))
	TObjectPtr<UButton> MainMenuButton;

	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, Category = "EndScreenWidget",
		meta = (ToolTip = "Restarts the current level"))
	void OnRestartClicked();

	UFUNCTION(BlueprintCallable, Category = "EndScreenWidget",
		meta = (ToolTip = "Transitions the game back to the main menu"))
	void OnMainMenuClicked();
};

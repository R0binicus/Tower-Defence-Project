#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

class UButton;

/**
 * The primary widget in the main menu
 */
UCLASS()
class TURRETMASTER_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainMenuWidget")
	const TSoftObjectPtr<UWorld> Level01;

	UPROPERTY(BlueprintReadOnly, Category = "MainMenuWidget", meta = (BindWidget))
	TObjectPtr<UButton> Level01Button;

	UPROPERTY(BlueprintReadOnly, Category = "MainMenuWidget", meta = (BindWidget))
	TObjectPtr<UButton> QuitButton;

	void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, Category = "MainMenuWidget",
		meta = (ToolTip = "Starts the first level"))
	void OnLevel01Clicked();

	UFUNCTION(BlueprintCallable, Category = "MainMenuWidget",
		meta = (ToolTip = "Quits the game application"))
	void OnQuitClicked();
};

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "MainMenuWidget.generated.h"

/**
 * The primary widget in the main menu
 */
UCLASS()
class TURRETMASTER_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainMenuWidget")
	const TSoftObjectPtr<UWorld> FirstLevel;

	UPROPERTY(BlueprintReadOnly, Category = "MainMenuWidget", meta = (BindWidget))
	TObjectPtr<UButton> PlayButton;

	UPROPERTY(BlueprintReadOnly, Category = "MainMenuWidget", meta = (BindWidget))
	TObjectPtr<UButton> QuitButton;

	void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, Category = "MainMenuWidget",
		meta = (ToolTip = "Starts the first level"))
	void OnPlayClicked();

	UFUNCTION(BlueprintCallable, Category = "MainMenuWidget",
		meta = (ToolTip = "Quits the game application"))
	void OnQuitClicked();
};

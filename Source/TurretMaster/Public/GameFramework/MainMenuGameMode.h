#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UI/MainMenuWidget.h"
#include "MainMenuGameMode.generated.h"

/**
 * Game mode for the tower defence game's main menu
 */
UCLASS()
class TURRETMASTER_API AMainMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainMenuGameMode")
	TSubclassOf<UMainMenuWidget> MainMenuWidgetClass;

	UPROPERTY(BlueprintReadWrite, Category = "MainMenuGameMode")
	TObjectPtr<UMainMenuWidget> MainMenuWidget;

	virtual void BeginPlay() override;
};

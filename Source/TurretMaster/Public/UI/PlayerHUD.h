#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "UI/BuildingsWidget.h"
#include "PlayerHUD.generated.h"

/**
 * Heads up display that appears on the players screen during gameplay
 * Contains other widgets
 */
UCLASS()
class TURRETMASTER_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()

protected:

	// Bind Widget

	UPROPERTY(BlueprintReadOnly, Category = "Player HUD", meta = (BindWidget))
	TObjectPtr<UCanvasPanel> MainCanvasPanel;

	UPROPERTY(BlueprintReadOnly, Category = "Player HUD", meta = (BindWidget))
	TObjectPtr<UBuildingsWidget> BuildingsWidget;
};

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HudWidget.generated.h"

class UCanvasPanel;
class UBuildingsWidget;

/**
 * Heads up display that appears on the players screen during gameplay
 * Contains other widgets
 */
UCLASS()
class TURRETMASTER_API UHudWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	// Bind Widget

	UPROPERTY(BlueprintReadOnly, Category = "HUD Widget", meta = (BindWidget))
	TObjectPtr<UCanvasPanel> MainCanvasPanel;

	UPROPERTY(BlueprintReadOnly, Category = "HUD Widget", meta = (BindWidget))
	TObjectPtr<UBuildingsWidget> BuildingsWidget;
};

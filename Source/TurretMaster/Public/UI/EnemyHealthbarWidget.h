#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyHealthbarWidget.generated.h"

class UProgressBar;

/**
 * Health bar widget, which displays an enemy's health over its head
 */
UCLASS()
class TURRETMASTER_API UEnemyHealthbarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "EnemyHealthbarWidget",
		meta = (ToolTip = "Updates the health bar to display a certain fill percentage"))
	void SetBarPercent(const float FillPercent);

protected:
	UPROPERTY(BlueprintReadWrite, Category = "EnemyHealthbarWidget", meta = (BindWidget))
	TObjectPtr<UProgressBar> HealthProgressBar;
};

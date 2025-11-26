#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuBaseWidget.generated.h"

class UButton;

/**
 * Base class for menu widget to inherit from
 */
UCLASS()
class TURRETMASTER_API UMenuBaseWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintCallable, Category = "EndScreenWidget",
		meta = (ToolTip = "Makes the end screen widget to be visible, or not visible"))
	void SetWidgetVisible(bool bIsVisible);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "EndScreenWidget", meta = (BindWidget))
	TObjectPtr<UButton> DefaultButton = nullptr;

	UFUNCTION(BlueprintCallable, Category = "EndScreenWidget",
		meta = (ToolTip = "Makes the game focus on the default set button"))
	virtual void FocusDefaultButton();
};

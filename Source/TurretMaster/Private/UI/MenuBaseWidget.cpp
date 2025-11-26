#include "UI/MenuBaseWidget.h"
#include "Components/Button.h"

void UMenuBaseWidget::NativeConstruct()
{
	if (BackgroundButton)
	{
		BackgroundButton->OnClicked.AddDynamic(this, &UMenuBaseWidget::FocusDefaultButton);
	}
}

void UMenuBaseWidget::SetWidgetVisible(const bool bIsVisible)
{
	if (bIsVisible)
	{
		FocusDefaultButton();
		SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		SetVisibility(ESlateVisibility::Hidden);
	}
}

void UMenuBaseWidget::FocusDefaultButton()
{
	if (DefaultButton)
	{
		DefaultButton->SetFocus();
	}
}

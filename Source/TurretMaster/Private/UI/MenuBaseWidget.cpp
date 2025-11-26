#include "UI/MenuBaseWidget.h"
#include "Components/Button.h"

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

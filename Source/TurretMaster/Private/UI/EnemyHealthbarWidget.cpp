#include "UI/EnemyHealthbarWidget.h"

void UEnemyHealthbarWidget::SetBarPercent(const float FillPercent)
{
	if (HealthProgressBar)
	{
		HealthProgressBar->SetPercent(FillPercent);
	}
}

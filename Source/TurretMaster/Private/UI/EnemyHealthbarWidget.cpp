#include "UI/EnemyHealthbarWidget.h"
#include "Components/ProgressBar.h"

void UEnemyHealthbarWidget::SetBarPercent(const float FillPercent) const
{
	if (HealthProgressBar)
	{
		HealthProgressBar->SetPercent(FillPercent);
	}
}

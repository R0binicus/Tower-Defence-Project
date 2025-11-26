#include "LimitedRepeatTimer.h"

void ULimitedRepeatTimer::SetupTimer(TObjectPtr<UWorld> InWorld, FTimerDelegate& InCallerDelagate, float InRate, int32 InRepeats)
{
	if (InRepeats == 0)
	{
		return;
	}

	if (!InWorld)
	{
		return;
	}

	World = InWorld;
	RemainingRepeats = InRepeats;

	TimerDelagate.BindUObject(this, &ULimitedRepeatTimer::RepeatTimer, InCallerDelagate, InRate);

	World->GetTimerManager().ClearTimer(TimerHandle);
	World->GetTimerManager().SetTimer(TimerHandle, TimerDelagate, InRate, false);
}

void ULimitedRepeatTimer::RepeatTimer(FTimerDelegate CallerDelagate, float RepeatRate)
{
	CallerDelagate.ExecuteIfBound();

	RemainingRepeats--;
	if (RemainingRepeats <= 0)
	{
		return;
	}

	if (!World)
	{
		return;
	}

	World->GetTimerManager().ClearTimer(TimerHandle);
	World->GetTimerManager().SetTimer(TimerHandle, TimerDelagate, RepeatRate, false);
}

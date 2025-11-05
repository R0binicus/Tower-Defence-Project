#include "LimitedRepeatTimer.h"

//ULimitedRepeatTimer::ULimitedRepeatTimer()
//{
//}

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

	TimerDelagate.BindUObject(this, &ULimitedRepeatTimer::RepeatFunction, InCallerDelagate, InRate);

	InWorld->GetTimerManager().SetTimer(TimerHandle, TimerDelagate, InRate, false);
}

void ULimitedRepeatTimer::RepeatFunction(FTimerDelegate CallerDelagate, float RepeatRate)
{
	CallerDelagate.Execute();

	if (!World)
	{
		return;
	}

	World->GetTimerManager().SetTimer(TimerHandle, TimerDelagate, RepeatRate, false);
}

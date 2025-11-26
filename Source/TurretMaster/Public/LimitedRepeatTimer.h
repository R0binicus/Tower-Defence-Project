#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LimitedRepeatTimer.generated.h"

/**
 * A wrapper around a timer, which is set to repeat a timer a set number of times
 */
UCLASS()
class TURRETMASTER_API ULimitedRepeatTimer : public UObject
{
	GENERATED_BODY()

public:
	void SetupTimer(TObjectPtr<UWorld> InWorld, FTimerDelegate& InCallerDelagate, float InRate, int32 InRepeats);

protected:
	int32 RemainingRepeats;

	FTimerHandle TimerHandle;

	FTimerDelegate TimerDelagate;

	TObjectPtr<UWorld> World;

	void RepeatTimer(FTimerDelegate CallerDelagate, float RepeatRate);
};

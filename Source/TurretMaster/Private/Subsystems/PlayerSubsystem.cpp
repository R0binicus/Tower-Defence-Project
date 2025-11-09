#include "Subsystems/PlayerSubsystem.h"

void UPlayerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	PlayerLivesCurrent = PlayerLivesInitial;
	PlayerMoneyCurrent = PlayerMoneyInitial;
}

void UPlayerSubsystem::SetPlayerLivesCurrent(const int32 NewLives)
{
	PlayerLivesCurrent = NewLives;
	OnPlayerLivesChanged.Broadcast(PlayerLivesCurrent);
}

void UPlayerSubsystem::SetPlayerMoneyCurrent(const int32 NewMoney)
{
	PlayerMoneyCurrent = NewMoney;
	OnPlayerMoneyChanged.Broadcast(PlayerMoneyCurrent);
}

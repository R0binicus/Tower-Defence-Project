#include "GameFramework/TowerDefencePlayerState.h"

ATowerDefencePlayerState::ATowerDefencePlayerState()
{
    PlayerLivesCurrent = PlayerLivesInitial;
    PlayerMoneyCurrent = PlayerMoneyInitial;
}

bool ATowerDefencePlayerState::TrySetPlayerState(const EPlayerStateEnum NewState)
{
    // This is excessive for what I actually need but it is extensible
    switch (PlayerStateEnum)
    {
    case EPlayerStateEnum::Default:
        SetPlayerState(NewState);
        return true;
        break;
    case EPlayerStateEnum::Building:
        SetPlayerState(NewState);
        return true;
        break;
    default:
        return false;
        break;
    }
}

void ATowerDefencePlayerState::SetPlayerState(const EPlayerStateEnum NewState)
{
    PlayerStateEnum = NewState;
    OnPlayerStateChanged.Broadcast(PlayerStateEnum);
}

void ATowerDefencePlayerState::SetPlayerLivesCurrent(const int32 NewLives)
{
    PlayerLivesCurrent = NewLives;
    OnPlayerLivesChanged.Broadcast(PlayerLivesCurrent);
}

void ATowerDefencePlayerState::SetPlayerMoneyCurrent(const int32 NewMoney)
{
    PlayerMoneyCurrent = NewMoney;
    OnPlayerMoneyChanged.Broadcast(PlayerMoneyCurrent);
}

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

void ATowerDefencePlayerState::ChangeCurrentMoney(const int32 MoneyChange)
{
    SetPlayerMoneyCurrent(PlayerMoneyCurrent + MoneyChange);
}

void ATowerDefencePlayerState::ChangeCurrentLives(const int32 LiveChange)
{
    SetPlayerLivesCurrent(PlayerLivesCurrent + LiveChange);
}

void ATowerDefencePlayerState::SetPlayerMoneyCurrent(const int32 NewMoney)
{
    PlayerMoneyCurrent = NewMoney;
    OnPlayerMoneyChanged.Broadcast(PlayerMoneyCurrent);
}

bool ATowerDefencePlayerState::HasEnoughResources(const int32 Cost) const
{
    const int32 ResourcesAfterCost = PlayerMoneyCurrent - Cost;
    if (ResourcesAfterCost < 0)
    {
        return false;
    }
    else
    {
        return true;
    }
}

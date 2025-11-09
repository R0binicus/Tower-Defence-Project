#include "GameFramework/TowerDefencePlayerState.h"

bool ATowerDefencePlayerState::TrySetPlayerState(const EPlayerStateEnum NewState)
{
    // This is excessive for what I actually need but oh well
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

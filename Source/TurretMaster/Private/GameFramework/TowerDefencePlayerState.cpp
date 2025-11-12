#include "GameFramework/TowerDefencePlayerState.h"

void ATowerDefencePlayerState::BeginPlay()
{
    SetPlayerLivesCurrent(PlayerLivesInitial);
    SetPlayerMoneyCurrent(PlayerMoneyInitial);
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
    OnPlayerStateChanged.Broadcast(NewState, PlayerStateEnum);
    PlayerStateEnum = NewState;
}

void ATowerDefencePlayerState::SetPlayerLivesCurrent(const int32 NewLives)
{
    OnPlayerLivesChanged.Broadcast(NewLives, PlayerLivesCurrent);
    PlayerLivesCurrent = NewLives;

    if (PlayerLivesCurrent > 0)
    {
        return;
    }

    TObjectPtr<ATowerDefenceGameState> GameState = Cast<ATowerDefenceGameState>(GetWorld()->GetGameState());
    if (GameState)
    {
        GameState->TwiggerLose();
    }
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
    OnPlayerMoneyChanged.Broadcast(NewMoney, PlayerMoneyCurrent);
    PlayerMoneyCurrent = NewMoney;
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

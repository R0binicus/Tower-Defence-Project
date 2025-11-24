#include "GameFramework/TowerDefencePlayerState.h"
#include "Subsystems/BuildingSubsystem.h"
#include "GameFramework/TowerDefenceGameState.h"
#include "Turret.h"
#include "GameFramework/TowerDefencePlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "DataAssets/BuildingDataAsset.h"

void ATowerDefencePlayerState::BeginPlay()
{
    SetPlayerLivesCurrent(PlayerLivesInitial);
    SetPlayerMoneyCurrent(PlayerMoneyInitial);

    const TObjectPtr<UBuildingSubsystem> BuildingSubsystem = GetWorld()->GetSubsystem<UBuildingSubsystem>();
    if (BuildingSubsystem)
    {
        BuildingSubsystem->OnBuildingHighlighted.AddUniqueDynamic(this, &ATowerDefencePlayerState::UpdateCurrentSelection);
    }

    const TObjectPtr<ATowerDefencePlayerController> PlayerController = Cast<ATowerDefencePlayerController>(UGameplayStatics::GetPlayerController(this, 0));
    if (PlayerController)
    {
        PlayerController->OnDeselectInput.AddDynamic(this, &ATowerDefencePlayerState::OnDeselectedAction);
    }
}

bool ATowerDefencePlayerState::TrySetPlayerState(const EPlayerStateEnum NewState)
{
    // This is excessive for what I actually need, but it is extensible
    switch (PlayerStateEnum)
    {
    case EPlayerStateEnum::Default:
        SetPlayerState(NewState);
        return true;
    case EPlayerStateEnum::Building:
        SetPlayerState(NewState);
        return true;
    default:
        return false;
    }
}

void ATowerDefencePlayerState::SetPlayerState(const EPlayerStateEnum NewState)
{
    OnPlayerStateChanged.Broadcast(NewState, PlayerStateEnum);
    PlayerStateEnum = NewState;
}

void ATowerDefencePlayerState::OnDeselectedAction()
{
    if (!SelectedTurret)
    {
        return;
    }

    const TObjectPtr<UBuildingSubsystem> BuildingSubsystem = GetWorld()->GetSubsystem<UBuildingSubsystem>();
    if (BuildingSubsystem)
    {
        BuildingSubsystem->OnBuildingHighlighted.Broadcast(nullptr, nullptr); // Also calls UpdateCurrentSelection
    }
}

void ATowerDefencePlayerState::UpdateCurrentSelection(UBuildingDataAsset* BuildingData, ATurret* Turret)
{
    SelectedTurretData = BuildingData;
    SelectedTurret = Turret;
}

void ATowerDefencePlayerState::SetPlayerLivesCurrent(const int32 NewLives)
{
    OnPlayerLivesChanged.Broadcast(NewLives, PlayerLivesCurrent);
    PlayerLivesCurrent = NewLives;

    if (PlayerLivesCurrent > 0)
    {
        return;
    }

    const TObjectPtr<ATowerDefenceGameState> GameState = Cast<ATowerDefenceGameState>(GetWorld()->GetGameState());
    if (GameState)
    {
        GameState->TriggerLose();
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
    
    return ResourcesAfterCost < 0;
}

void ATowerDefencePlayerState::SellBuilding()
{
    if (!SelectedTurretData)
    {
        return;
    }

    ChangeCurrentMoney(SelectedTurretData->Cost * SellReturnFraction);

    SelectedTurret->Destroy();
    UpdateCurrentSelection(nullptr, nullptr);
}

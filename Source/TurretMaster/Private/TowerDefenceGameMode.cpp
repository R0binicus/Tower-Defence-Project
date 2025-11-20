#include "TowerDefenceGameMode.h"
#include "GameFramework/TowerDefenceGameInstance.h"
#include "Subsystems/BuildingSubsystem.h"
#include "Subsystems/EnemySubsystem.h"

ATowerDefenceGameMode::ATowerDefenceGameMode()
{
	PlayerStateClass = ATowerDefencePlayerState::StaticClass();
	GameStateClass = ATowerDefenceGameState::StaticClass();
	HUDClass = ATowerDefenceHUD::StaticClass();
}

void ATowerDefenceGameMode::BeginPlay()
{
	UTowerDefenceGameInstance* GameInstance = Cast<UTowerDefenceGameInstance>(GetWorld()->GetGameInstance());
	TObjectPtr<UEnemySubsystem> EnemySubsystem = GetWorld()->GetSubsystem<UEnemySubsystem>();
	TObjectPtr<UBuildingSubsystem> BuildingSubsystem = GetWorld()->GetSubsystem<UBuildingSubsystem>();
	if (!GameInstance || !EnemySubsystem || !BuildingSubsystem)
	{
		return;
	}

	EnemySubsystem->StartSubsystem();
	BuildingSubsystem->StartSubsystem();
	GameInstance->LoadDataUsingLevel(GetWorld());
}

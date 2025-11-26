#include "TowerDefenceGameMode.h"
#include "GameFramework/TowerDefenceGameInstance.h"
#include "GameFramework/TowerDefenceGameState.h"
#include "Subsystems/BuildingSubsystem.h"
#include "Subsystems/EnemySubsystem.h"
#include "GameFramework/TowerDefenceHUD.h"
#include "GameFramework/TowerDefencePlayerState.h"

ATowerDefenceGameMode::ATowerDefenceGameMode()
{
	PlayerStateClass = ATowerDefencePlayerState::StaticClass();
	GameStateClass = ATowerDefenceGameState::StaticClass();
	HUDClass = ATowerDefenceHUD::StaticClass();
}

void ATowerDefenceGameMode::BeginPlay()
{
	const TObjectPtr<UEnemySubsystem> EnemySubsystem = GetWorld()->GetSubsystem<UEnemySubsystem>();
	const TObjectPtr<UBuildingSubsystem> BuildingSubsystem = GetWorld()->GetSubsystem<UBuildingSubsystem>();
	const TObjectPtr<UTowerDefenceGameInstance> GameInstance = Cast<UTowerDefenceGameInstance>(GetWorld()->GetGameInstance());
	if (!GameInstance || !EnemySubsystem || !BuildingSubsystem)
	{
		return;
	}

	EnemySubsystem->StartSubsystem();
	BuildingSubsystem->StartSubsystem();
	GameInstance->LoadDataUsingLevel(GetWorld());
}

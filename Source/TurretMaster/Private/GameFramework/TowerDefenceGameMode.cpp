#include "GameFramework/TowerDefenceGameMode.h"
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
	TObjectPtr<UWorld> World = GetWorld();
	const TObjectPtr<UEnemySubsystem> EnemySubsystem = World->GetSubsystem<UEnemySubsystem>();
	const TObjectPtr<UBuildingSubsystem> BuildingSubsystem = World->GetSubsystem<UBuildingSubsystem>();
	const TObjectPtr<UTowerDefenceGameInstance> GameInstance = Cast<UTowerDefenceGameInstance>(World->GetGameInstance());
	if (!GameInstance || !EnemySubsystem || !BuildingSubsystem)
	{
		return;
	}

	EnemySubsystem->StartSubsystem();
	BuildingSubsystem->StartSubsystem(ButtonClickSound, ErrorSound);
	GameInstance->LoadDataUsingLevel(World);
}

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "GameFramework/TowerDefencePlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "LivesLossArea.generated.h"

/**
 * Area in the level which reduces player lives whenan enemy enters it
 */
UCLASS()
class TURRETMASTER_API ALivesLossArea : public AActor
{
	GENERATED_BODY()
	
public:	
	ALivesLossArea();

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "LivesLossArea")
	TObjectPtr<UBoxComponent> EnemyDetectionBox;

	UPROPERTY(EditDefaultsOnly, Category = "Turret")
	FName EnemyTagName = "Enemy";

	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};

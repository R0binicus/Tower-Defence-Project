#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UCLASS()
class TURRETMASTER_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemy();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Enemt",
		meta = (ToolTip = "Sets the enemy's destination"))
	void SetDestination(const FVector NewDestination);

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
};

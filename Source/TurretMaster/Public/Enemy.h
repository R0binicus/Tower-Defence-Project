#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Damageable.h"
#include "Enemy.generated.h"

UCLASS()
class TURRETMASTER_API AEnemy : public ACharacter, public IDamageable
{
	GENERATED_BODY()

public:
	AEnemy();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Enemy",
		meta = (ToolTip = "Sets the enemy's destination"))
	void SetDestination(const FVector NewDestination);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	float MaxHealth = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	float CurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	bool bIsDead = false;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	void TakeDamage_Implementation(float DamageTaken);

	bool IsDead_Implementation() const { return bIsDead; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Damagable")
	void Death();
	void Death_Implementation();
};

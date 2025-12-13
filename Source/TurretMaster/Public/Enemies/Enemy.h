#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/Damageable.h"
#include "Enemy.generated.h"

class UWidgetComponent;
class UEnemyHealthbarWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyDeath, int32, ResourcesGained);

UCLASS()
class TURRETMASTER_API AEnemy : public ACharacter, public IDamageable
{
	GENERATED_BODY()

public:
	AEnemy();

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Enemy")
	FOnEnemyDeath OnEnemyDeath;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Enemy",
		meta = (ToolTip = "Sets the enemy's LivesReduction"))
	int32 GetLivesReduction() const { return LivesReduction;  };

	UFUNCTION(BlueprintCallable, Category = "Enemy",
		meta = (ToolTip = "Sets the enemy's destination"))
	void SetDestination(const FVector NewDestination) const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Damagable")
	void Death(const bool bByTurret);
	void Death_Implementation(const bool bByTurret);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	TObjectPtr<UWidgetComponent> HealthBarWidgetComponent;

	// Widget is set in the enemy's HealthBarWidgetComponent inspector, as it refuses to be set in C++
	UPROPERTY(BlueprintReadOnly, Category = "Enemy")
	TObjectPtr<UEnemyHealthbarWidget> HealthBarWidget;

	UPROPERTY(BlueprintReadWrite, Category = "Enemy")
	TObjectPtr<APlayerCameraManager> CameraManager;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy")
	FVector2D HealthbarScaleMultiplier = FVector2D(0.5, 0.3);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy")
	FVector2D HealthbarScaleMin = FVector2D(0.1, 0.3);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	int32 LivesReduction = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	float MaxHealth = 100.f;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Enemy")
	float CurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	int32 ResourcesOnKill = 5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	bool bIsDead = false;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	void TakeDamage_Implementation(const float DamageTaken);

	bool IsDead_Implementation() const { return bIsDead; }
};

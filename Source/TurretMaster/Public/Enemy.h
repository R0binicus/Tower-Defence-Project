#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Damageable.h"
#include "Components/WidgetComponent.h"
#include "UI/EnemyHealthbarWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Enemy.generated.h"

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
	void SetDestination(const FVector NewDestination);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Damagable")
	void Death(bool bByTurret);
	void Death_Implementation(bool bByTurret);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	TObjectPtr<UWidgetComponent> HealthBarWidgetComponent;

	// Widget is set in the enemy's HealthBarWidgetComponent inspector, as it refuses to be set in C++
	UPROPERTY(BlueprintReadOnly, Category = "Enemy")
	TObjectPtr<UEnemyHealthbarWidget> HealthBarWidget;

	UPROPERTY(BlueprintReadWrite, Category = "Enemy")
	TObjectPtr<APlayerCameraManager> CameraManager;

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

	void TakeDamage_Implementation(float DamageTaken);

	bool IsDead_Implementation() const { return bIsDead; }
};

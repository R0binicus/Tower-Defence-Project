#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "TeslaProjectile.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;

// TODO: Make this into a class which doesn't inherit from AProjectile, and make a 'TurretSpawnable' interface

/**
 * Projectile variant for the tesla turret
 */
UCLASS()
class TURRETMASTER_API ATeslaProjectile : public AProjectile
{
	GENERATED_BODY()

public:
	ATeslaProjectile();

	virtual void SetProjectileEnabled(bool bNewEnabled) override;

	virtual void SetupProjectile(AEnemy* Enemy, const FProjectileValues& InProjectileValues) override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TeslaProjectile")
	int32 AllowedTeslaBounces = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TeslaProjectile")
	TArray<TObjectPtr<UNiagaraComponent>> LightningBeamVFXArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TeslaProjectile")
	TEnumAsByte<EObjectTypeQuery> EnemyCollisionType = ObjectTypeQuery8;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TeslaProjectile")
	class UNiagaraSystem* LightningVFX;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "TeslaProjectile",
		meta = (ToolTip = "Finds a set number of valid enemies, deals damage to them, and makes lightning VFX appear between them"))
	void BounceToTargets(const int32 NumberOfTargets);

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "TeslaProjectile",
		meta = (ToolTip = "Finds a valid enemy to bounce to, then bounces to it"))
	AEnemy* FindClosestEnemy(const FVector& CheckOriginPoint, const TArray<AEnemy*>& AlreadyTargetedEnemies) const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "TeslaProjectile",
		meta = (ToolTip = "Finds a valid enemy to bounce to, then bounces to it"))
	void CreateLightningVFX(const FVector& StartPosition, const FVector& EndPosition);
};

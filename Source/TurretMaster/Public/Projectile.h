#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class UStaticMeshComponent;
class UProjectileMovementComponent;

/**
 *
 */
UCLASS()
class TURRETMASTER_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AProjectile();

	UFUNCTION(BlueprintCallable, Category = "Projectile",
		meta = (ToolTip = "Gets the damage that the projectile will deal to its target"))
	float GetProjectileDamage() const { return Damage; }

	UFUNCTION(BlueprintCallable, Category = "Projectile",
		meta = (ToolTip = "Sets the projectile's initial values"))
	void SetProjectileValues(AActor* Target);

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Projectile",
		meta = (ToolTip = "Sets the projectile's initial values"))
	void MoveToTarget();

	virtual void MoveToTarget_Implementation();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Projectile")
	TWeakObjectPtr<AActor> TargetActor = nullptr;

	UPROPERTY(VisibleDefaultsOnly, Category = "Projectile")
	TObjectPtr<UStaticMeshComponent> CollisionMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	TObjectPtr<UProjectileMovementComponent> MovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	float Damage = 25.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	float Lifetime = 10.0f;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};

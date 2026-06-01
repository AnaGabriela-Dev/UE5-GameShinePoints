
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Fireball.generated.h"

class ACharacterBase;
class USphereComponent;
class UProjectileMovementComponent;

/*
 * Projectile actor used by FireballAbility.
 *
 * This class represents the projectile behavior
 * after being spawned by the ability.
 */
UCLASS()
class SKILLSYSTEM_API AFireball : public AActor
{
	GENERATED_BODY()
	
public:	
	AFireball();

public:	
	void SetDamage(float InDamage);
	USphereComponent* GetCollisionComponent() const;
	
protected:
	virtual void BeginPlay() override;
	
	/** Call explosions effects */
	void Explode();
	
protected:
	/** Call when Fireball hit something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, FVector NormalImpulse, const 
		FHitResult& Hit);
	
protected:
	/** Collision component */
	UPROPERTY(VisibleAnywhere)
	USphereComponent* CollisionComp = nullptr;
	
	/** Movement Component */
	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* ProjectileMovement = nullptr;

	UPROPERTY(EditAnywhere, Category = "Effects")
	UParticleSystem* ExplosionEffect = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Effects")
	USoundBase* ExplosionSound = nullptr;

private:
	float DamageValue = 0.0f;
};

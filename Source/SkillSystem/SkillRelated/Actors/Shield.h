#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Shield.generated.h"

class UNiagaraComponent;
class USphereComponent;
/**
 * Defensive actor spawned by ShieldAbility.
 *
 * This actor acts as a temporary defensive barrier.
 */
UCLASS()
class SKILLSYSTEM_API AShield : public AActor
{
	GENERATED_BODY()
	
public:	
	AShield();
	
public:
	AActor* GetShieldOwner() const;

	USphereComponent* GetCollisionComponent() const;

protected:
	virtual void BeginPlay() override;
	
protected:
	/** Collision component */
	UPROPERTY(VisibleAnywhere)
	USphereComponent* CollisionComp = nullptr;
	
	/** Niagara VFX component used for the shield visuals. */
	UPROPERTY(VisibleAnywhere)
	UNiagaraComponent* ShieldVFX = nullptr;
	
	UPROPERTY()
	AActor* ShieldOwner = nullptr;
};

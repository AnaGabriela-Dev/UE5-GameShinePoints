
#pragma once

#include "CoreMinimal.h"
#include "SkillRelated/Ability.h"
#include "ShieldAbility.generated.h"

class UNiagaraSystem;
class AShield;
class UNiagaraComponent;

/**
 * Ability responsible for spawning a temporary shield actor.
 *
 * This class handles the activation logic,
 * while the ShieldActor handles collision behavior.
 */
UCLASS()
class SKILLSYSTEM_API UShieldAbility : public UAbility
{
	GENERATED_BODY()
	
public:
	virtual void Activate(AActor* Actor) override;
	
protected:
	/** Shield class to spawn. */
	UPROPERTY(EditDefaultsOnly, Category = "Shield")
	TSubclassOf<AShield> ShieldClass;

	/** Lifetime of the shield.*/
	UPROPERTY(EditDefaultsOnly, Category = "Shield")
	float ShieldDuration = 2.f;

};

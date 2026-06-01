
#pragma once

#include "CoreMinimal.h"
#include "SkillRelated/Ability.h"
#include "FireballAbility.generated.h"

/**
 * Ability responsible for spawning and launching a Fireball projectile.
 * 
 * This class represents the "casting logic",
 * not the projectile behavior itself.
 */
UCLASS()
class SKILLSYSTEM_API UFireballAbility : public UAbility
{
	GENERATED_BODY()

public:
	UFireballAbility();
	
public:
	virtual void Activate(AActor* Actor) override;

protected:
	/** Project class that we are going to generate Fireball*/
	UPROPERTY(EditAnywhere, Category = "Fireball")
	TSubclassOf<AActor> ProjectileClass = nullptr;
	
};

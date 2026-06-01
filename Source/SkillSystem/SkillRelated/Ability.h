#pragma once

#include "CoreMinimal.h"
#include "Ability.generated.h"

class ACharacterBase;
class UAbilityDataAsset;
class UResourceComponent;

/**
 * Define all fundamental data's for a skill.
 */
UCLASS(Abstract, Blueprintable, BlueprintType)
class SKILLSYSTEM_API UAbility : public UObject
{
	GENERATED_BODY()

public:
	UAbility();

public:
	/** Checks if the ability can be activated (has enough resources) */
	UFUNCTION(BlueprintCallable, Category = "Ability")
	virtual bool CanActivate(AActor* Actor);

	/** Executes the ability logic and consumes resources */
	UFUNCTION(BlueprintCallable, Category = "Ability")
	virtual void Activate(AActor* Actor);
	
	/** Function responsible to return if cooldown is still running */
	UFUNCTION(BlueprintCallable, Category = "Ability")
	bool IsOnCooldown() const;
	
	/** Function responsible to turn Cooldown as false */
	UFUNCTION(BlueprintCallable, Category = "Ability")
	void ResetCooldown();
	
	/** Return Remaining Cooldown for future UI */
	UFUNCTION(BlueprintPure, Category = "Ability")
	float GetRemainingCooldown() const;
	
	/** Return Damage */
	UFUNCTION(BlueprintPure, Category = "Ability")
	float GetDamage() const;
	
	/** Return Attack Range */
	UFUNCTION(BlueprintCallable, Category = "Ability Properties")
	float GetAttackRange() const;
	
	float GetCooldownDuration() const;
	
public: 
	void Initialize(UAbilityDataAsset* InData);

protected:
	/** UAbilityDataAsset infos */
	UPROPERTY(BlueprintReadOnly, Category = "Ability|Data")
	UAbilityDataAsset* Data = nullptr;
	
	UPROPERTY()
	TObjectPtr<ACharacterBase> OwnerCharacter = nullptr;

protected:
	FTimerHandle CooldownTimerHandle;
	float LastActivationTime = -10.0f;
	bool bIsOnCooldown = false;
	
};
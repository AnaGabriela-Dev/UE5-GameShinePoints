// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "AbilityManagerComponent.generated.h"


class UVM_AbilitySlot;
class UInputAction;
class UAbilityDataAsset;
class UAbilitySetDataAsset;
class UAbility;

/** 
 * Responsible for storing, organizing, and controlling the skills available 
 * to an actor.
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SKILLSYSTEM_API UAbilityManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAbilityManagerComponent();

public:
	/** Add a new Skill in manager */
	UFUNCTION(BlueprintCallable, Category = "Abilities|Actions")
	void AddAbility(const FGameplayTag AbilityTag, UAbilityDataAsset* AbilityData);

	/** Remove a new Skill from manager */
	UFUNCTION(BlueprintCallable, Category = "Abilities|Actions")
	void RemoveAbility(const FGameplayTag AbilityTag);

	/** Activate a Skill based on Tag */
	UFUNCTION(BlueprintCallable, Category = "Abilities|Actions")
	bool ActivateAbility(const FGameplayTag AbilityTag);
	
	/** Function with all the requisites to activate Ability */
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	bool TryActivateAbility(const FGameplayTag AbilityTag);
	
	/** 
	 * Translates a triggered Input Action into a Gameplay Tag and attempts to 
	 * activate the associated ability.
	 */
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void ProcessAbilityInput(const UInputAction* Action);
	
	UFUNCTION(BlueprintCallable)
	bool TryActivateAbilityByTag(FGameplayTag AbilityTag);
	
	UFUNCTION(BlueprintCallable)
	bool CanUseAbilityByTag(FGameplayTag AbilityTag);
	
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	float GetAbilityRangeByTag(FGameplayTag AbilityTag);
	
	UFUNCTION(BlueprintCallable)
	UAbility* GetAbilityByTag(FGameplayTag AbilityTag) const;

public:
	void UpdateAbilityViewModels() const;
	void InitializeAbilityViewModels() const;
	void SetViewModels(UVM_AbilitySlot* InFireballVM,
		UVM_AbilitySlot* InShieldVM);

protected:
	virtual void BeginPlay() override;
	
protected:
	/** Live ability instances for tracking cooldowns and state during 
	 * gameplay. 
	 */
	UPROPERTY(VisibleInstanceOnly, Category = "Abilities")
	TMap<FGameplayTag, UAbility*> AvailableAbilities;
	
	/** Data Asset defining the initial set of abilities (Loadout). */
	UPROPERTY(EditAnywhere, Category = "Abilities")
	UAbilitySetDataAsset* AbilitySet = nullptr;
	
	UPROPERTY()
	TMap<const UInputAction*, FGameplayTag> InputToTagMap;
	
	UPROPERTY(BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<UVM_AbilitySlot> FireballVM = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<UVM_AbilitySlot> ShieldVM = nullptr;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag FireballTag = FGameplayTag::RequestGameplayTag(
		TEXT("Ability.Active.FireBall"));

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag ShieldTag = FGameplayTag::RequestGameplayTag(
		TEXT("Ability.Active.Shield"));

private:
	FTimerHandle CooldownUpdateTimer;
};

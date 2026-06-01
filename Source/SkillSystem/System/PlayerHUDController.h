// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerHUDController.generated.h"

class UVM_AbilitySlot;
class UVM_Score;
class UVM_Resource;
class UVM_Health;
class ASkillSystemPlayerController;
/**
 * Coordinates the initialization of the player HUD and manages access to its associated ViewModels.
 */
UCLASS()
class SKILLSYSTEM_API UPlayerHUDController : public UObject
{
	GENERATED_BODY()
	
public:
	void Initialize(ASkillSystemPlayerController* InPlayerController,
		TSubclassOf<UUserWidget> InHUDClass);
	
public:

	UVM_Health* GetHealthVM() const;
	UVM_Resource* GetManaVM() const;
	UVM_Score* GetScoreVM() const;
	UVM_AbilitySlot* GetFireballVM() const;
	UVM_AbilitySlot* GetShieldVM() const;

private:
	UPROPERTY()
	TObjectPtr<UUserWidget> PlayerHUD = nullptr;
	
	UPROPERTY()
	TObjectPtr<UVM_Health> HealthVM = nullptr;

	UPROPERTY()
	TObjectPtr<UVM_Resource> ManaVM = nullptr;

	UPROPERTY()
	TObjectPtr<UVM_Score> ScoreVM = nullptr;
	
	UPROPERTY()
	TObjectPtr<UVM_AbilitySlot> FireballVM = nullptr;

	UPROPERTY()
	TObjectPtr<UVM_AbilitySlot> ShieldVM = nullptr;
	
private:
	TWeakObjectPtr<APlayerController> PlayerController = nullptr;
};

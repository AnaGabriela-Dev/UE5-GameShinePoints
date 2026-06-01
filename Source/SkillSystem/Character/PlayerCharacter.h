

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "InputActionValue.h"
#include "InputAction.h"
#include "PlayerCharacter.generated.h"

class UScoreComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionInstance;

/**
 * Character controlled by the local player.
 * Handles:
 * - Camera
 * - Input
 * - HUD initialization
 * - Movement
 * - Sprint
 */
UCLASS()
class SKILLSYSTEM_API APlayerCharacter : public ACharacterBase
{
	GENERATED_BODY()

public:
	APlayerCharacter();
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UScoreComponent> ScoreComponent = nullptr;

protected:
	virtual void PossessedBy(AController* NewController) override;
	virtual void SetupPlayerInputComponent(UInputComponent* 
		PlayerInputComponent) override;
	
	/** INPUT FUNCTIONS */
	void Look(const FInputActionValue& Value);
	void Move(const FInputActionValue& Value);
	void StartSprint(const FInputActionValue& Value);
	void StopSprint(const FInputActionValue& Value);
	void Input_Ability(const FInputActionInstance& Instance);
	void QuitGame(const FInputActionValue& Value);
	
	/** DEATH */
	virtual void HandleDeath() override;
	
	virtual FVector GetAbilityAimDirection() const override;
	
protected:
	/** CAMERA */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera")
	TObjectPtr<USpringArmComponent> CameraBoom = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera")
	TObjectPtr<UCameraComponent> FollowCamera = nullptr;

	/** INPUTS */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> MoveAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> LookAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> JumpAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> SprintAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> AbilityAction = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> QuitAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* AbilityAction_2 = nullptr;
	
	/** UI */
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> RespawnCooldownWidget = nullptr;
};
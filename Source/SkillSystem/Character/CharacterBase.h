
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

class UHealthComponent;
class VM_Resource;
struct FInputActionInstance;
class UAbilityManagerComponent;
class USpringArmComponent;     
class UCameraComponent;       
class UInputMappingContext;   
class UInputAction;       
class UResourceComponent;
class USceneComponent;

UENUM(BlueprintType)
enum class ETeam : uint8
{
	Player,
	Enemy
};

/** 
 * This class is responsible for character characteristics BASE
 */
UCLASS(Abstract)
class SKILLSYSTEM_API ACharacterBase : public ACharacter
{
	
	GENERATED_BODY()

public:
	ACharacterBase();
	
public:
	USceneComponent* GetFireballSpawnPoint() const;
	UResourceComponent* GetResourceComponent() const;
	virtual FVector GetAbilityAimDirection() const;
	
	ETeam GetTeam() const;
	
	/** Play animations functions */
	virtual void PlayMontage(UAnimMontage* Montage);
	virtual void PlayHitReaction();
	virtual void PlayAbilityAnimation(UAnimMontage* Montage);
	virtual void PlayDeathAnimation();
	virtual void PlayRespawnAnimation();
	
protected:
	/** Just a base function that childs will need to have */
	UFUNCTION()
	virtual void HandleDeath();
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UHealthComponent> HealthComp = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UResourceComponent> ManaComp = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UAbilityManagerComponent> AbilityManager = nullptr;

protected:
	virtual void BeginPlay() override;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> FireballSpawnPoint = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations")
	UAnimMontage* DeathAnimMontage = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations")
	UAnimMontage* RespawnAnimMontage = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations")
	UAnimMontage* HitReactionAnimMontage = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category="Team")
	ETeam Team = ETeam::Enemy;
};
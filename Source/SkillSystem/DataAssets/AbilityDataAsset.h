#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AbilityDataAsset.generated.h"

/** 
 * The data asset responsible for storing skill information, allowing it to be 
 * manipulated in UE5 without needing to access code.
 */
UCLASS(BlueprintType)
class SKILLSYSTEM_API UAbilityDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General")
	FName AbilityName = "";
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	float ResourceCost = 20.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	float CooldownDuration = 5.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	float Damage = 60.0f;
	
	/** Max Attack Range for Ability */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	float AttackRange = 500.0f;

	/**  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Logic")
	TSubclassOf<class UAbility> AbilityClass = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* CastAnimation = nullptr;
};
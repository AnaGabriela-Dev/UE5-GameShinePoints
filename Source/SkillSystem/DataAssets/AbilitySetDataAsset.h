
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "AbilitySetDataAsset.generated.h"

class UInputAction;
class UAbilityDataAsset;
/**
 * This class allows the Ability Manager to identify and instantiate abilities 
 * using tags instead of hardcoded class references.
 */
USTRUCT(BlueprintType)
struct FAbilityMapping
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag AbilityTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAbilityDataAsset* AbilityData = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputAction* InputAction = nullptr;
};

UCLASS()
class SKILLSYSTEM_API UAbilitySetDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	TArray<FAbilityMapping> AbilityList;
};

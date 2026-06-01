
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ResourceComponent.generated.h"

class UVM_Resource;

/** 
 * This class is responsible for resource management
 * (mana, stamina...)
 * 
 * This component takes care of consume, restore and valor validation of 
 * resource.
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SKILLSYSTEM_API UResourceComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UResourceComponent();
	
public:
	UFUNCTION(BlueprintCallable, Category = "Resources")
	void ConsumeResource(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Resources")
	void RestoreResource(const float Amount);
	
	UFUNCTION(BlueprintCallable, Category = "Resources")
	bool HasEnoughResource(float ResourceCost) const;
	
public: 
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, 
		FActorComponentTickFunction* ThisTickFunction) override;
	
	float GetMaxResource() const;
	
	void SetCurrentResource(const float Amount);
	void SetViewModel(UVM_Resource* InHealthVM);
	
protected:
	virtual void BeginPlay() override;
	
	/** Handles cleanup during EndPlay by clearing all active timers
	 * associated with this component. */
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
private:
	/** Handles the logic for recovering the resource; called by the timer. */
	void RegenerateResource();
	
	/** Starts or resets the regeneration timer with an initial delay. */
	void StartRegenTimer(const float StartDelay, const float TickDelay);
	
	/** Synchronizes the current state of the ResourceComponent with the 
	 * ViewModel. 
	 */
	void UpdateViewModel() const;
	
	
public:
	/** Handle for the regeneration timer */
	FTimerHandle RegenTimerHandle;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Resources")
	float MaxResource = 200.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Resources")
	float CurrentResource = 0.0f;
	
	/** Define if Debug will be visible */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resources|Debug")
	bool bShowDebug = false;
	
	/** Amount of regeneration per second*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resources|Regen")
	float RegenRate = 5.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resources|Regen")
	float RegenStartDelay = 2.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resources|Regen")
	float RegenTickDelay = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resources|Regen")
	bool bEnableRegen = true;
	
	/** Reference to the ViewModel that manages resource data for the UI. */
	UPROPERTY()
	UVM_Resource* ResourceVM = nullptr;
	
protected:
	bool bIsVMReady = false;
	
};

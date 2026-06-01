#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

class UWidgetComponent;
class UVM_Health;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);

/** 
 * This class is responsible for health management
 * 
 * This component takes care of damage and health stuffs 
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SKILLSYSTEM_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHealthComponent();
	
public:
	UFUNCTION(BlueprintCallable, Category = "Health")
	void TakeDamage(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Health")
	void RestoreHealth(const float Amount);
	
public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, 
		FActorComponentTickFunction* ThisTickFunction) override;
	
	/** Initializes the enemy widget by binding the Health ViewModel to its 
	 * MVVM view extension. */
	void InitializeEnemyWidget(UWidgetComponent* WidgetComponent);
	
	/** Finds the owner's widget component and rebinds the Health ViewModel 
	 * to its MVVM view. */
	void RebindEnemyWidget();
	
	float GetCurrentHealth() const;
	float GetMaxHealth() const;
	
	void SetbIsDead(bool newIsDead);
	void SetCurrentHealth(float NewHealth);
	void SetViewModel(UVM_Health* InHealthVM);
	
protected:
	virtual void BeginPlay() override;
	
	/** Handles cleanup during EndPlay by clearing all active timers associated 
	 * with this component. */
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
private:
	/** Handles the logic for recovering the health; called by the timer. */
	void RegenerateHealth();
	
	/** Starts or resets the regeneration timer with an initial delay. */
	void StartRegenTimer(const float StartDelay, const float TickDelay);
	
	/** Synchronizes the current state of the ResourceComponent with the 
	 * ViewModel. 
	 */
	void UpdateViewModel() const;
	
public:
	UPROPERTY(BlueprintAssignable, Category = "Health")
	FOnDeath OnDeath;
	
	/** Reference to the ViewModel that manages health data for the UI. */
	UPROPERTY()
	UVM_Health* HealthVM = nullptr;
	
public:
	/** Handle for the regeneration timer */
	FTimerHandle RegenTimerHandle;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
	float MaxHealth = 100.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Health")
	float CurrentHealth = 100.0f;
	
	/** Amount of regeneration per second*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health|Regen")
	float RegenRate = 20.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health|Regen")
	float RegenStartDelay = 5.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health|Regen")
	float RegenTickDelay = 5.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health|Regen")
	bool bEnableRegen = true;
	
	/** Define if Debug will be visible */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health|Debug")
	bool bShowDebug = false;
	
private:
	bool bIsDead = false;
	bool bIsVMReady = false;
};

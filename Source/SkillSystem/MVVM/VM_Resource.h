
#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "VM_Resource.generated.h"

/**
 * A ViewModel class designed to bridge character resource data (like Mana or 
 * Health) to the UI using the Model-View-ViewModel (MVVM) pattern.
 *
 * It utilizes FieldNotify to reactively update UI elements whenever the 
 * CurrentResource or MaxResource values change.
 */
UCLASS(BlueprintType)
class SKILLSYSTEM_API UVM_Resource : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, FieldNotify, Category = "ViewModel")
	float GetResourcePercent() const;
	
	UFUNCTION(BlueprintPure, FieldNotify, Category = "Resource")
	FText GetManaText() const;
	
public:
	void SetCurrentResource(float NewValue);
	void SetMaxResource(float NewValue);
	
protected:
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Category = "ViewModel")
	float CurrentResource = 0.0f;

	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Category = "ViewModel")
	float MaxResource = 0.0f;
	
	UPROPERTY(BlueprintReadOnly, Category = "Resource")
	FText ManaText = FText::AsCultureInvariant("Mana:");
};
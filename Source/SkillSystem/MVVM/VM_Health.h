// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "VM_Health.generated.h"

/**
 * A ViewModel class designed to bridge character health data to the UI using 
 * the Model-View-ViewModel (MVVM) pattern.
 *
 * It utilizes FieldNotify to reactively update UI elements whenever the 
 * CurrentHealth or MaxHealth values change.
 */
UCLASS()
class SKILLSYSTEM_API UVM_Health : public UMVVMViewModelBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, FieldNotify, Category = "ViewModel")
	float GetHealthPercent() const;
	
	UFUNCTION(BlueprintPure, FieldNotify, Category = "Health")
	FText GetHealthText() const;
	
public:
	void SetCurrentHealth(float NewValue);
	void SetMaxHealth(float NewValue);
	
protected:
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Category = "ViewModel")
	float CurrentHealth = 0.0f;

	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Category = "ViewModel")
	float MaxHealth = 0.0f;
	
	UPROPERTY(BlueprintReadOnly, Category = "Health")
	FText HealthText = FText::AsCultureInvariant("Vida:");
};

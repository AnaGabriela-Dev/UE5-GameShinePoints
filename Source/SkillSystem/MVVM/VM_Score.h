// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "VM_Score.generated.h"

/**
 * 
 */
UCLASS()
class SKILLSYSTEM_API UVM_Score : public UMVVMViewModelBase
{
	GENERATED_BODY()
	
public:
	void SetScoreText(int Value);
	
	UFUNCTION(BlueprintPure, FieldNotify)
	FText GetScoreText() const;
	
protected:
	UPROPERTY(FieldNotify)
	int CurrentScore = 0;
	
};

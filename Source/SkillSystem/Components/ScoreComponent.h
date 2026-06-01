// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MVVM/VM_Score.h"
#include "ScoreComponent.generated.h"

/**
 * 
 * 
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SKILLSYSTEM_API UScoreComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UScoreComponent();
	
public:
	/** Add points to score */
	UFUNCTION(BlueprintCallable)
	void AddPoints(int Amount);

	UFUNCTION(BlueprintCallable)
	int GetScore() const;
	
public:
	void UpdateViewModel() const;
	void SetViewModel(UVM_Score* InHealthVM);
	
protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(BlueprintReadOnly)
	int CurrentScore = 0;
	
	UPROPERTY()
	UVM_Score* ScoreVM = nullptr;
};


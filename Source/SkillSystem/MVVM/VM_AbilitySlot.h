#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "VM_AbilitySlot.generated.h"

UCLASS()
class SKILLSYSTEM_API UVM_AbilitySlot : public UMVVMViewModelBase
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintPure, FieldNotify, Category = "ViewModel")
    float GetCooldownPercent() const;

    UFUNCTION(BlueprintPure, FieldNotify, Category = "ViewModel")
    FText GetCooldownText() const;

public:
    void SetCurrentCooldown(float NewValue);
    void SetMaxCooldown(float NewValue);

protected:

    UPROPERTY(BlueprintReadWrite, FieldNotify, Category = "ViewModel")
    float CurrentCooldown = 0.0f;

    UPROPERTY(BlueprintReadWrite, FieldNotify, Category = "ViewModel")
    float MaxCooldown = 0.0f;
};
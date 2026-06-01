#include "VM_AbilitySlot.h"

void UVM_AbilitySlot::SetCurrentCooldown(float NewValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(CurrentCooldown, NewValue);

	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetCooldownPercent);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetCooldownText);
}

void UVM_AbilitySlot::SetMaxCooldown(float NewValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(MaxCooldown, NewValue);

	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetCooldownPercent);
}

float UVM_AbilitySlot::GetCooldownPercent() const
{
	if (MaxCooldown <= 0.f)
	{
		return 0.f;
	}

	return FMath::Clamp(CurrentCooldown / MaxCooldown, 0.f, 1.f);
}

FText UVM_AbilitySlot::GetCooldownText() const
{
    if (CurrentCooldown <= 0.f)
    {
        return FText::FromString(TEXT("PRONTO"));
    }

    return FText::AsNumber(
        FMath::CeilToInt(CurrentCooldown));
}

#include "VM_Health.h"

void UVM_Health::SetCurrentHealth(float NewValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(CurrentHealth, NewValue);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetHealthPercent);
}

void UVM_Health::SetMaxHealth(float NewValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(MaxHealth, NewValue);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetHealthPercent);
}

float UVM_Health::GetHealthPercent() const
{
	if (MaxHealth <= 0.0f)
	{
		return 0.0f;
	}
	return FMath::Clamp(CurrentHealth / MaxHealth, 0.0f, 1.0f);
}

FText UVM_Health::GetHealthText() const
{
	return HealthText;
}

#include "VM_Resource.h"

void UVM_Resource::SetCurrentResource(float NewValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(CurrentResource, NewValue);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetResourcePercent);
}

void UVM_Resource::SetMaxResource(float NewValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(MaxResource, NewValue);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetResourcePercent);
}

float UVM_Resource::GetResourcePercent() const
{
	if (MaxResource <= 0.0f)
	{
		return 0.0f;
	}
	return FMath::Clamp(CurrentResource / MaxResource, 0.0f, 1.0f);
}

FText UVM_Resource::GetManaText() const
{
	return ManaText;
}

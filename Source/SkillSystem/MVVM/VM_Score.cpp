

#include "MVVM/VM_Score.h"

void UVM_Score::SetScoreText(int Value)
{
	UE_MVVM_SET_PROPERTY_VALUE(CurrentScore, Value);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetScoreText);
}

FText UVM_Score::GetScoreText() const
{
	return FText::AsNumber(CurrentScore);
}

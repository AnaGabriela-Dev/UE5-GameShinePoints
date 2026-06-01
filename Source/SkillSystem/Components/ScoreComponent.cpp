
#include "Components/ScoreComponent.h"

#include "MVVM/VM_Score.h"


UScoreComponent::UScoreComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UScoreComponent::BeginPlay()
{
	Super::BeginPlay();
	UpdateViewModel();
}

void UScoreComponent::AddPoints(int Amount)
{
	CurrentScore += Amount;

	UpdateViewModel();
}

void UScoreComponent::UpdateViewModel() const
{
	if (!ScoreVM)
	{
		return;
	}

	ScoreVM->SetScoreText(CurrentScore);
}


void UScoreComponent::SetViewModel(UVM_Score* InScoreVM)
{
	ScoreVM = InScoreVM;
	UpdateViewModel();
}

int UScoreComponent::GetScore() const
{
	return CurrentScore;
}



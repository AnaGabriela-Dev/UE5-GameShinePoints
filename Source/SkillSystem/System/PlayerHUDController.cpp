
#include "System/PlayerHUDController.h"
#include "System/SkillSystemPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "MVVM/VM_Health.h"
#include "MVVM/VM_Resource.h"
#include "MVVM/VM_Score.h"
#include "MVVM/VM_AbilitySlot.h"
#include "View/MVVMView.h"

void UPlayerHUDController::Initialize(
	ASkillSystemPlayerController* InPlayerController,
	TSubclassOf<UUserWidget> InHUDClass)
{
	PlayerController = InPlayerController;

	// Create ViewModels
	HealthVM = NewObject<UVM_Health>(this);
	ManaVM   = NewObject<UVM_Resource>(this);
	ScoreVM  = NewObject<UVM_Score>(this);
	FireballVM = NewObject<UVM_AbilitySlot>(this);
	ShieldVM   = NewObject<UVM_AbilitySlot>(this);

	// Create HUD
	PlayerHUD = CreateWidget<UUserWidget>(PlayerController.Get(),
		InHUDClass);
	if (!PlayerHUD)
	{
		return;
	}

	// Get MVVM Extension
	UMVVMView* View = PlayerHUD->GetExtension<UMVVMView>();
	if (!View)
	{
		return;
	}

	// Register ViewModels
	View->SetViewModel(FName("HealthViewModel"),HealthVM);
	View->SetViewModel(FName("ManaViewModel"),ManaVM);
	View->SetViewModel(FName("ScoreViewModel"),ScoreVM);
	View->SetViewModel(FName("FireballViewModel"),FireballVM);
	View->SetViewModel(FName("ShieldViewModel"),ShieldVM);

	PlayerHUD->AddToViewport();
}

UVM_Health* UPlayerHUDController::GetHealthVM() const
{
	return HealthVM;
}

UVM_Resource* UPlayerHUDController::GetManaVM() const
{
	return ManaVM;
}

UVM_Score* UPlayerHUDController::GetScoreVM() const
{
	return ScoreVM;
}

UVM_AbilitySlot* UPlayerHUDController::GetFireballVM() const
{
	return FireballVM;
}

UVM_AbilitySlot* UPlayerHUDController::GetShieldVM() const
{
	return ShieldVM;
}
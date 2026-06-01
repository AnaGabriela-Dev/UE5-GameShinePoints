// Copyright Epic Games, Inc. All Rights Reserved.


#include "SkillSystemPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "Blueprint/UserWidget.h"
#include "SkillSystem.h"
#include "Character/PlayerCharacter.h"
#include "Components/HealthComponent.h"
#include "Components/ResourceComponent.h"
#include "Components/ScoreComponent.h"
#include "SkillRelated/AbilityManagerComponent.h"
#include "Widgets/Input/SVirtualJoystick.h"
#include "System/PlayerHUDController.h"
#include "ScoreSystem/EndGameWidget.h"

void ASkillSystemPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (SVirtualJoystick::ShouldDisplayTouchInterface() && IsLocalPlayerController())
	{
		MobileControlsWidget = CreateWidget<UUserWidget>(this, 
			MobileControlsWidgetClass);

		if (MobileControlsWidget)
		{
			// add the controls to the player screen
			MobileControlsWidget->AddToPlayerScreen(0);

		} else {

			UE_LOG(LogSkillSystem, Error, TEXT("Could not spawn mobile "
									  "controls widget."));

		}

	}
}

void ASkillSystemPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (!HUDController)
	{
		HUDController = NewObject<UPlayerHUDController>(this);
		HUDController->Initialize(this, HUDClass);
	}

	APlayerCharacter* PossessedCharacter = Cast<APlayerCharacter>(InPawn);
	if (!PossessedCharacter || !HUDController)
	{
		return;
	}

	PossessedCharacter->HealthComp->SetViewModel(HUDController->GetHealthVM());
	PossessedCharacter->ManaComp->SetViewModel(HUDController->GetManaVM());
	PossessedCharacter->ScoreComponent->SetViewModel(HUDController->GetScoreVM());
	if (PossessedCharacter->AbilityManager)
	{
		PossessedCharacter->AbilityManager->SetViewModels(
			HUDController->GetFireballVM(), HUDController->GetShieldVM());
	}
}

void ASkillSystemPlayerController::ShowEndGameScreen(
	int32 FinalScore)
{
	if (!EndGameWidgetClass)
	{
		return;
	}

	UEndGameWidget* EndScreen = CreateWidget<UEndGameWidget>(
		this,
		EndGameWidgetClass);

	if (!EndScreen)
	{
		return;
	}

	EndScreen->FinalScore = FinalScore;
	EndScreen->AddToViewport();
	
	bShowMouseCursor = true;

	FInputModeGameAndUI InputMode;
	SetInputMode(InputMode);
}

UPlayerHUDController* ASkillSystemPlayerController::GetHUDController() const
{
	return HUDController;
}

void ASkillSystemPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// only add IMCs for local player controllers
	if (IsLocalPlayerController())
	{
		// Add Input Mapping Contexts
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = 
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>
			(GetLocalPlayer()))
		{
			for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
			{
				Subsystem->AddMappingContext(CurrentContext, 0);
			}

			// only add these IMCs if we're not using mobile touch input
			if (!SVirtualJoystick::ShouldDisplayTouchInterface())
			{
				for (UInputMappingContext* CurrentContext : MobileExcludedMappingContexts)
				{
					Subsystem->AddMappingContext(CurrentContext, 0);
				}
			}
		}
	}
}

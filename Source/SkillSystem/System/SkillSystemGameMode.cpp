// Copyright Epic Games, Inc. All Rights Reserved.

#include "SkillSystemGameMode.h"

#include "Character/CharacterBase.h"
#include "GameFramework/PlayerController.h"

void ASkillSystemGameMode::RequestRespawn(AController* Controller)
{
	if (!Controller) {
		return;
	}
	
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, [this, Controller]()
	{
		RestartPlayer(Controller);

			APawn* NewPawn = Controller->GetPawn();

			ACharacterBase* NewCharacter = Cast<ACharacterBase>(NewPawn);
			if (!NewCharacter)
			{
				return;
			}
		
			NewCharacter->PlayRespawnAnimation();
	}, RespawnDelay, false);
}

float ASkillSystemGameMode::GetRespawnDelay() const
{
	return RespawnDelay;
}
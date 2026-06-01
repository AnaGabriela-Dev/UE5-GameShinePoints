// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SkillSystemGameMode.generated.h"

/**
 *  Simple GameMode for a third person game
 */
UCLASS(abstract)
class ASkillSystemGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	/** Called when player dies */
	void RequestRespawn(AController* Controller);
	
	float GetRespawnDelay() const;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Respawn")
	float RespawnDelay = 3.0f;
};




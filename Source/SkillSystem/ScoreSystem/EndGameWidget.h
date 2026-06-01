#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EndGameWidget.generated.h"

/** 
 * Manages the end-game UI screen and displays the player's final 
 * performance statistics.
 */
UCLASS()
class SKILLSYSTEM_API UEndGameWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category="EndGame")
	int FinalScore = 0;
};
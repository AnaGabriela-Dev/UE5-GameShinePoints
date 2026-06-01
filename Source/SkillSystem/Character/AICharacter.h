
#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "AICharacter.generated.h"

class UResourceComponent;
class UWidgetComponent;
class UHealthComponent;
class UAbilityManagerComponent;
class USceneComponent;

/**
 * This class is responsible for handle Enemies Logic
 */
UCLASS()
class SKILLSYSTEM_API AAICharacter : public ACharacterBase
{
	GENERATED_BODY()

public:
	AAICharacter();

protected:
	virtual void BeginPlay() override;
	virtual void HandleDeath() override;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> EnemyHealthWidget = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category="Score")
	int ScoreValue = 200;
	
};
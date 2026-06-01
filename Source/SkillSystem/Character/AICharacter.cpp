
#include "Character/AICharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "Components/HealthComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/SceneComponent.h"
#include "Components/ScoreComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MVVM/VM_Health.h"

AAICharacter::AAICharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	Team = ETeam::Enemy;

	EnemyHealthWidget = CreateDefaultSubobject<UWidgetComponent>(
		TEXT("EnemyLifeWidgetBar"));
	EnemyHealthWidget->SetupAttachment(RootComponent);

	FireballSpawnPoint->SetRelativeLocation(
		FVector(40.f, 0.f, 30.f));
	
	// COLLISION
	GetCharacterMovement()->bEnablePhysicsInteraction = false;
	GetCharacterMovement()->PushForceFactor = 0.f;
}

void AAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
	HealthComp->OnDeath.RemoveDynamic(this, &AAICharacter::HandleDeath);
	HealthComp->OnDeath.AddDynamic(this, &AAICharacter::HandleDeath);

	UVM_Health* EnemyVM = NewObject<UVM_Health>(this);

	EnemyVM->SetMaxHealth(HealthComp->GetMaxHealth());
	EnemyVM->SetCurrentHealth(HealthComp->GetCurrentHealth());

	HealthComp->SetViewModel(EnemyVM);
	HealthComp->InitializeEnemyWidget(EnemyHealthWidget);
}


void AAICharacter::HandleDeath()
{
	HealthComp->OnDeath.RemoveDynamic(this, &AAICharacter::HandleDeath);
	
	if (AAIController* AIController = Cast<AAIController>(GetController()))
	{
		AIController->BrainComponent->StopLogic("Enemy Died"); 
		AIController->ClearFocus(EAIFocusPriority::Gameplay);
	}
	
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		if (APawn* PlayerPawn = PC->GetPawn())
		{
			if (UScoreComponent* ScoreComp = PlayerPawn->FindComponentByClass
					<UScoreComponent>())
			{
				ScoreComp->AddPoints(ScoreValue);
			}
		}
	}
	
	Super::HandleDeath();
	
	if (EnemyHealthWidget) 
	{
		EnemyHealthWidget->SetVisibility(false);
	}
	
	SetLifeSpan(1.0f);
}



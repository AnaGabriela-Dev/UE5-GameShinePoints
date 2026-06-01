
#include "Components/HealthComponent.h"
#include "Blueprint/UserWidget.h"
#include "Character/CharacterBase.h"
#include "Components/WidgetComponent.h"
#include "MVVM/VM_Health.h"
#include "View/MVVMView.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
	
	UpdateViewModel();
}

void UHealthComponent::TakeDamage(float Amount)
{
	if (!GetOwner() || !GetWorld())
	{
		return;
	}
	
	
	CurrentHealth = FMath::Clamp(CurrentHealth - Amount, 0.0f, 
		MaxHealth);

	if (HealthVM && bIsVMReady)
	{
		HealthVM->SetCurrentHealth(CurrentHealth);
	}
	
	if (!GetWorld()->GetTimerManager().IsTimerActive(RegenTimerHandle))
	{
		StartRegenTimer(RegenStartDelay, RegenTickDelay);
	}
	
	if (CurrentHealth <= 0.0f && !bIsDead)
	{
		bIsDead = true;
		OnDeath.Broadcast();
		return;
	}
	
	ACharacterBase* Character = Cast<ACharacterBase>(GetOwner());
	if (!Character)
	{
		return;
	}
	Character->PlayHitReaction();
}

void UHealthComponent::RestoreHealth(const float Amount)
{
	if (!GetOwner() || !GetWorld())
	{
		return;
	}
	
	CurrentHealth = FMath::Clamp(CurrentHealth + Amount, 0.0f, 
		MaxHealth);
	if (HealthVM && bIsVMReady)
	{
		HealthVM->SetCurrentHealth(CurrentHealth);
	}
	
	if (CurrentHealth == MaxHealth)
	{
		GetWorld()->GetTimerManager().ClearTimer(RegenTimerHandle);
	}
}

void UHealthComponent::RegenerateHealth()
{
	if (!bEnableRegen || CurrentHealth >= MaxHealth)
	{
		return;
	}
	
	RestoreHealth(RegenRate);
}

void UHealthComponent::StartRegenTimer(const float StartDelay, const float 
	TickDelay)
{
	if (!bEnableRegen)
	{
		return;
	}
	
	GetWorld()->GetTimerManager().SetTimer(RegenTimerHandle, this, 
		&UHealthComponent::RegenerateHealth, TickDelay, true, 
		StartDelay);
}

void UHealthComponent::UpdateViewModel() const
{
	if (!HealthVM)
	{
		return;
	}
	
	HealthVM->SetMaxHealth(MaxHealth);
	HealthVM->SetCurrentHealth(CurrentHealth);
}

void UHealthComponent::InitializeEnemyWidget(UWidgetComponent* WidgetComponent)
{
	if (!WidgetComponent || !HealthVM)
	{
		return;
	}

	UUserWidget* Widget = WidgetComponent->GetUserWidgetObject();
	if (!Widget)
	{
		return;
	}

	UMVVMView* View = Widget->GetExtension<UMVVMView>();
	if (!View)
	{
		return;
	}

	View->SetViewModel(FName("HealthViewModel"),HealthVM);
}

void UHealthComponent::RebindEnemyWidget()
{
	if (!HealthVM)
	{
		return;
	}

	AActor* Owner = GetOwner();
	if (!Owner)
	{
		return;
	}

	UWidgetComponent* WidgetComp =
		Owner->FindComponentByClass<UWidgetComponent>();

	if (!WidgetComp)
	{
		return;
	}

	UUserWidget* Widget = WidgetComp->GetUserWidgetObject();
	if (!Widget)
	{
		return;
	}

	UMVVMView* View = Widget->GetExtension<UMVVMView>();
	if (!View)
	{
		return;
	}

	View->SetViewModel(FName("HealthViewModel"), HealthVM);
}

void UHealthComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (!GetWorld())
	{
		return;
	}
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}


float UHealthComponent::GetCurrentHealth() const
{
	return CurrentHealth;
}

float UHealthComponent::GetMaxHealth() const
{
	return MaxHealth;
}

void UHealthComponent::SetbIsDead(bool newIsDead)
{
	bIsDead = newIsDead;
}

void UHealthComponent::SetCurrentHealth(float NewHealth){
	CurrentHealth = NewHealth;
}

void UHealthComponent::SetViewModel(UVM_Health* InHealthVM)
{
	HealthVM = InHealthVM;
	bIsVMReady = true;
	
	UpdateViewModel();
	RebindEnemyWidget();
}
 
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, 
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (!bShowDebug)
	{
		return;
	}
	
	if (GEngine)
	{
		FString DebugMsg = FString::Printf(TEXT("[Health]: %.0f / "
										  "%.0f"), CurrentHealth, MaxHealth);
		GEngine->AddOnScreenDebugMessage(1, 0.0f, FColor::
			Green, DebugMsg);
	}
}


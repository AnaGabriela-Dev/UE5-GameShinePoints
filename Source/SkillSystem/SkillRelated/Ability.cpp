#include "Ability.h"

#include "Character/CharacterBase.h"
#include "Components/ResourceComponent.h"
#include "DataAssets/AbilityDataAsset.h"

UAbility::UAbility()
{
}

bool UAbility::CanActivate(AActor* Actor)
{
	if (!Data)
	{
		UE_LOG(LogTemp, Error, TEXT("[Ability] Erro: Tentativa de ativar "
							  "habilidade sem Data Asset!"));
		return false;
	}
	
	if (!Actor)
	{
		return false;
	}
	
	UResourceComponent* ResourceComp = Actor->FindComponentByClass
		<UResourceComponent>();
	if (!ResourceComp)
	{
		return false;
	}
	
	if (IsOnCooldown()) 
	{
		return false;
	}
	
	return ResourceComp->HasEnoughResource(Data->ResourceCost);
}

void UAbility::Activate(AActor* Actor)
{
	if (!Data)
	{
		UE_LOG(LogTemp, Error, TEXT("[Ability] Erro: Tentativa de ativar "
							  "habilidade sem Data Asset!"));
		return;
	}
	
	if (!Actor)
	{
		UE_LOG(LogTemp, Error, TEXT("[Ability] Ponteiro Nulo para Actor!"));
		return;
	};
	
	
	if (!CanActivate(Actor))
	{
		return;
	}
	
	UResourceComponent* ResourceComp = Actor->FindComponentByClass
		<UResourceComponent>();
	if (!ResourceComp)
	{
		return;
	}
	ResourceComp->ConsumeResource(Data->ResourceCost);
	
	bIsOnCooldown = true;
	GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle, this, 
		&UAbility::ResetCooldown,Data->CooldownDuration, false);
	
	OwnerCharacter = Cast<ACharacterBase>(Actor);
	if (OwnerCharacter && Data->CastAnimation)
	{
		OwnerCharacter->PlayAbilityAnimation(Data->CastAnimation);
	}
	
	UE_LOG(LogTemp, Log, TEXT("[Ability] %s Ativada! Próximo uso em: %.1f s"), 
		*Data->AbilityName.ToString(), Data->CooldownDuration);
}

bool UAbility::IsOnCooldown() const
{
	return bIsOnCooldown;
}

void UAbility::ResetCooldown()
{
	bIsOnCooldown = false;
	const FString Name = Data ? Data->AbilityName.ToString() : TEXT("Unknown");
	UE_LOG(LogTemp, Log, TEXT("[Ability] %s: Cooldown finalizado."), *Name);
}

float UAbility::GetRemainingCooldown() const
{
	if (!bIsOnCooldown)
	{
		return 0.0f;
	};
	
	constexpr float MinCooldownValue = 0.0f;
	
	const float Remaining = GetWorld()->GetTimerManager().GetTimerRemaining
		(CooldownTimerHandle);
    
	return FMath::Max(Remaining, MinCooldownValue);
}

void UAbility::Initialize(UAbilityDataAsset* InData)
{
	if (!InData)
	{
		UE_LOG(LogTemp, Error, TEXT("[Ability] Tentativa de inicializar com "
							  "Data Asset NULO!"));
		return;
	}
	Data = InData;
}

float UAbility::GetDamage() const
{
	if (!Data)
	{
		return 0.0f;
	}
	
	return Data->Damage;
}

float UAbility::GetAttackRange() const
{
	if (!Data)
	{
		return 0.0f;
	}
	return Data->AttackRange;
}

float UAbility::GetCooldownDuration() const
{
	if (!Data)
	{
		return 0.0f;
	}
	return Data->CooldownDuration;
}

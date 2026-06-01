
#include "SkillRelated/AbilityManagerComponent.h"
#include "Ability.h"
#include "GameplayTagContainer.h"
#include "MVVM/VM_AbilitySlot.h"
#include "DataAssets/AbilityDataAsset.h"
#include "DataAssets/AbilitySetDataAsset.h"

UAbilityManagerComponent::UAbilityManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAbilityManagerComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (!AbilitySet)
	{
		UE_LOG(LogTemp, Warning, TEXT("[AbilityManager] AbilitySet não "
								"configurado!"));
		return;
	}
	
	for (const FAbilityMapping& Mapping : AbilitySet->AbilityList)
	{
		if (!Mapping.AbilityData || !Mapping.AbilityTag.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("[AbilityManager] Item inválido no "
								 "Data Asset!"));
			continue;
		}
		
		InputToTagMap.Add(Mapping.InputAction, Mapping.AbilityTag);
		AddAbility(Mapping.AbilityTag, Mapping.AbilityData);
	}
}

void UAbilityManagerComponent::AddAbility(const FGameplayTag AbilityTag, 
	UAbilityDataAsset* AbilityData)
{
	if (!AbilityTag.IsValid() || !AbilityData || !AbilityData->AbilityClass) 
	{
		return;
	}
	
	if (AvailableAbilities.Contains(AbilityTag))
	{
		return;
	}
	
	UAbility* NewAbility = NewObject<UAbility>(this, AbilityData->
		AbilityClass);
	if (!NewAbility)
	{
		return;
	}
	
	NewAbility->Initialize(AbilityData);
	AvailableAbilities.Add(AbilityTag, NewAbility);
}

void UAbilityManagerComponent::RemoveAbility(const FGameplayTag AbilityTag)
{
	if (!AbilityTag.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("[AbilityManager] Tentativa de remover "
								"tag inválida."));
		return;
	}
	if (!AvailableAbilities.Contains(AbilityTag))
	{
		UE_LOG(LogTemp, Warning, TEXT("[AbilityManager] Habilidade %s não "
								"possuída"), *AbilityTag.ToString());
		return;
	}
	
	AvailableAbilities.Remove(AbilityTag);
	UE_LOG(LogTemp, Log, TEXT("[AbilityManager] Habilidade %s removida."), 
		*AbilityTag.ToString());
}

bool UAbilityManagerComponent::ActivateAbility(const FGameplayTag AbilityTag)
{
	if (!AvailableAbilities.Contains(AbilityTag))
	{
		UE_LOG(LogTemp, Warning, TEXT("[AbilityManager] Habilidade %s não "
								"encontrada!"), *AbilityTag.ToString());
		return false;
	}
	
	UE_LOG(LogTemp, Log, TEXT("[AbilityManager] Ativando Habilidade: %s"), 
		*AbilityTag.ToString());
	return true;
}

bool UAbilityManagerComponent::TryActivateAbility(
	const FGameplayTag AbilityTag)
{
	if (!AbilityTag.IsValid() || !AvailableAbilities.Contains(AbilityTag))
	{
		UE_LOG(LogTemp, Warning, TEXT("[AbilityManager] Falha: Tag inválida ou "
								"habilidade não possuída."));
		return false;
	}

	UAbility** const AbilityPtr = AvailableAbilities.Find(AbilityTag);
	if (!AbilityPtr || !(*AbilityPtr))
	{
		UE_LOG(LogTemp, Warning,
			TEXT("[AbilityManager] Habilidade %s não possuída."),
			*AbilityTag.ToString());

		return false;
	}

	UAbility* AbilityToUse = *AbilityPtr;

	AActor* OwnerActor = GetOwner();
	
	if (!AbilityToUse->CanActivate(OwnerActor))
	{
		return false;
	}

	AbilityToUse->Activate(OwnerActor);

	return true;
}

void UAbilityManagerComponent::ProcessAbilityInput(const UInputAction* Action)
{
	if (!Action)
	{
		return;
	}
	
	if (!InputToTagMap.Contains(Action))
	{
		return;
	}
	
	FGameplayTag TagFound = InputToTagMap[Action];
	TryActivateAbility(TagFound);
}

bool UAbilityManagerComponent::TryActivateAbilityByTag(
	FGameplayTag AbilityTag)
{
	return TryActivateAbility(AbilityTag);
}

bool UAbilityManagerComponent::CanUseAbilityByTag(FGameplayTag AbilityTag)
{
	UAbility** FoundAbility = AvailableAbilities.Find(AbilityTag);

	if (!FoundAbility || !(*FoundAbility))
	{
		return false;
	}

	return (*FoundAbility)->CanActivate(GetOwner());
}

float UAbilityManagerComponent::GetAbilityRangeByTag(FGameplayTag AbilityTag)
{
	UAbility** FoundAbility = AvailableAbilities.Find(AbilityTag);
	if (!FoundAbility || !(*FoundAbility))
	{
		return 0.0f;
	}
	
	return (*FoundAbility)->GetAttackRange();
}

UAbility* UAbilityManagerComponent::GetAbilityByTag(
	const FGameplayTag AbilityTag) const
{
	UAbility* const* FoundAbility = AvailableAbilities.Find(AbilityTag);
	if (!FoundAbility)
	{
		return nullptr;
	}
	
	return *FoundAbility;
}

void UAbilityManagerComponent::UpdateAbilityViewModels() const
{
	const UAbility* FireballAbility = GetAbilityByTag(FireballTag);
	const UAbility* ShieldAbility = GetAbilityByTag(ShieldTag);

	if (FireballAbility && FireballVM)
	{
		FireballVM->SetCurrentCooldown(
			FireballAbility->GetRemainingCooldown());
	}

	if (ShieldAbility && ShieldVM)
	{
		ShieldVM->SetCurrentCooldown(
			ShieldAbility->GetRemainingCooldown());
	}
}

void UAbilityManagerComponent::InitializeAbilityViewModels() const
{
	const UAbility* FireballAbility = GetAbilityByTag(FireballTag);
	const UAbility* ShieldAbility = GetAbilityByTag(ShieldTag);

	if (FireballAbility && FireballVM)
	{
		FireballVM->SetCurrentCooldown(
			FireballAbility->GetRemainingCooldown());

		FireballVM->SetMaxCooldown(
			FireballAbility->GetCooldownDuration());
	}

	if (ShieldAbility && ShieldVM)
	{
		ShieldVM->SetCurrentCooldown(
			ShieldAbility->GetRemainingCooldown());

		ShieldVM->SetMaxCooldown(
			ShieldAbility->GetCooldownDuration());
	}
}

void UAbilityManagerComponent::SetViewModels(UVM_AbilitySlot* InFireballVM,
	UVM_AbilitySlot* InShieldVM)
{
	FireballVM = InFireballVM;
	ShieldVM = InShieldVM;

	InitializeAbilityViewModels();
	GetWorld()->GetTimerManager().SetTimer(
		CooldownUpdateTimer,
		this,
		&UAbilityManagerComponent::UpdateAbilityViewModels,
		0.1f,
		true);
}

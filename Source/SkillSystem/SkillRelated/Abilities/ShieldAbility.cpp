
#include "SkillRelated/Abilities/ShieldAbility.h"
#include "Character/CharacterBase.h"
#include "SkillRelated/Actors/Shield.h"

void UShieldAbility::Activate(AActor* Actor)
{
	Super::Activate(Actor);
	
	if (!ShieldClass)
	{
		return;
	}
	
	if (!OwnerCharacter)
	{
		return;
	}
	
	AShield* Shield = GetWorld()->SpawnActor<AShield>(ShieldClass,
			OwnerCharacter->GetActorLocation(),FRotator::ZeroRotator);
	if (!Shield)
	{
		return;
	}
	
	Shield->SetOwner(OwnerCharacter);
	Shield->AttachToComponent(OwnerCharacter->GetRootComponent(),
		FAttachmentTransformRules::SnapToTargetIncludingScale);

	Shield->SetLifeSpan(ShieldDuration);
}

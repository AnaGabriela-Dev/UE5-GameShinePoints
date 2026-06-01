
#include "FireballAbility.h"
#include "Character/CharacterBase.h"
#include "SkillRelated/Actors/Fireball.h"
#include "DataAssets/AbilityDataAsset.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimMontage.h"
#include "Components/SphereComponent.h"
#include "SkillRelated/Actors/Shield.h"

UFireballAbility::UFireballAbility()
{
}

void UFireballAbility::Activate(AActor* Actor)
{
	Super::Activate(Actor);

	if (!OwnerCharacter)
	{
		return;
	}

	if (!Data || !ProjectileClass)
	{
		return;
	}

	USceneComponent* SpawnPoint = OwnerCharacter->GetFireballSpawnPoint();

	if (!SpawnPoint)
	{
		return;
	}

	const FVector SpawnLocation = SpawnPoint->GetComponentLocation();
	const FVector AimDirection = OwnerCharacter->GetAbilityAimDirection();
	const FRotator SpawnRotation = AimDirection.Rotation();

	AFireball* FireballProjectile = GetWorld()->SpawnActorDeferred<AFireball>(
			ProjectileClass,
			FTransform(SpawnRotation, SpawnLocation),
			Actor,
			Cast<APawn>(Actor),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	if (!FireballProjectile)
	{
		return;
	}
	FireballProjectile->SetOwner(Actor);
	
	// Shield owner no collision
	TArray<AActor*> AttachedActors;
	Actor->GetAttachedActors(AttachedActors);

	for (AActor* AttachedActor : AttachedActors)
	{
		AShield* Shield = Cast<AShield>(AttachedActor);

		if (!Shield)
		{
			continue;
		}

		FireballProjectile->GetCollisionComponent()->
			IgnoreActorWhenMoving(Shield, true);

		Shield->GetCollisionComponent()->
			IgnoreActorWhenMoving(FireballProjectile, true);
	}
	
	FireballProjectile->SetInstigator(Cast<APawn>(Actor));
	FireballProjectile->SetDamage(Data->Damage);

	UGameplayStatics::FinishSpawningActor(FireballProjectile,
		FTransform(SpawnRotation, SpawnLocation));
}
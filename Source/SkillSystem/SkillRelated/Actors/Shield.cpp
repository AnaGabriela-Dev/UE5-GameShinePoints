
#include "SkillRelated/Actors/Shield.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"

AShield::AShield()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(200.f);
	RootComponent = CollisionComp;
	
	ShieldVFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ShieldVFX"));
	ShieldVFX->SetupAttachment(RootComponent);

	InitialLifeSpan = 2.f;
}

void AShield::BeginPlay()
{
	Super::BeginPlay();
	
	if (AActor* OwnerActor = GetOwner())
	{
		CollisionComp->IgnoreActorWhenMoving(OwnerActor,true);
	}
}

USphereComponent* AShield::GetCollisionComponent() const
{
	return CollisionComp;
}

AActor* AShield::GetShieldOwner() const
{
	return ShieldOwner;
}
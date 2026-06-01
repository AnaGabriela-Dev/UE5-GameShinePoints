
#include "SkillRelated/Actors/Fireball.h"

#include "Shield.h"
#include "Character/AICharacter.h"
#include "Character/CharacterBase.h"
#include "Components/HealthComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"


AFireball::AFireball()
{
	PrimaryActorTick.bCanEverTick = false;
	
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(65.0f);
	RootComponent = CollisionComp;
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>
		(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.0f;
	ProjectileMovement->MaxSpeed = 3000.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->ProjectileGravityScale = 0.0f;
	
	CollisionComp->OnComponentHit.AddDynamic(this, &AFireball::OnHit);
	
	InitialLifeSpan = 5.0f;
}

void AFireball::BeginPlay()
{
	Super::BeginPlay();

	AActor* OwnerActor = GetOwner();
	if (OwnerActor)
	{
		CollisionComp->IgnoreActorWhenMoving(OwnerActor,true);
	}

	if (APawn* InstigatorPawn = GetInstigator())
	{
		CollisionComp->IgnoreActorWhenMoving(
			InstigatorPawn,
			true);
	}
	
	TArray<AActor*> AIActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),AAICharacter::StaticClass(),
		AIActors);

	if (Cast<AAICharacter>(OwnerActor))
	{
		for (AActor* AI : AIActors)
		{
			CollisionComp->IgnoreActorWhenMoving(AI, true);
			
		}
	}
}

void AFireball::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& 
	Hit)
{
	if (OtherActor == GetInstigator())
	{
		return;
	}
	
	if (!OtherActor || OtherActor == GetOwner())
	{
		return;
	}
	
	AShield* Shield = Cast<AShield>(OtherActor);
	if (Shield)
	{
		Explode();
		Destroy();
		return;
	}
	
	UHealthComponent* TargetHealth = OtherActor->FindComponentByClass
		<UHealthComponent>();
	
	if (Cast<AAICharacter>(GetOwner()) && Cast<AAICharacter>(OtherActor))
	{
		return;
	}
    
	if (TargetHealth)
	{
		TargetHealth->TakeDamage(DamageValue);
		UE_LOG(LogTemp, Log, TEXT("Bola de Fogo causou %.1f de dano em %s"), 
		   DamageValue, *OtherActor->GetName());
	}
	
	Explode();
	Destroy();
}

void AFireball::Explode()
{
	if (ExplosionEffect)
	{
		FVector ExplosionScale(3.0f, 3.0f, 3.0f); 
    
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, 
			GetActorLocation(), GetActorRotation(), ExplosionScale );
	}
	
	if (ExplosionSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, 
			ExplosionSound, GetActorLocation());
	}
}


void AFireball::SetDamage(float InDamage)
{
	DamageValue = InDamage;
}

USphereComponent* AFireball::GetCollisionComponent() const
{
	return CollisionComp;
}


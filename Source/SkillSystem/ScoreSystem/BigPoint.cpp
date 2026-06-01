#include "BigPoint.h"

#include "Character/PlayerCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ScoreComponent.h"
#include "Kismet/GameplayStatics.h"
#include "System/SkillSystemPlayerController.h"

ABigPoint::ABigPoint()
{
	PrimaryActorTick.bCanEverTick = false;

	Collision =
		CreateDefaultSubobject<USphereComponent>(
			TEXT("Collision"));

	RootComponent = Collision;

	Mesh =
		CreateDefaultSubobject<UStaticMeshComponent>(
			TEXT("Mesh"));

	Mesh->SetupAttachment(RootComponent);

	Collision->OnComponentBeginOverlap.AddDynamic(
		this,
		&ABigPoint::OnOverlap);
}

void ABigPoint::OnOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	APlayerCharacter* Player =
		Cast<APlayerCharacter>(OtherActor);

	if (!Player)
	{
		return;
	}

	UScoreComponent* ScoreComp =
		Player->FindComponentByClass<UScoreComponent>();

	if (!ScoreComp)
	{
		return;
	}
	
	if (bCollected)
	{
		return;
	}
	bCollected = true;

	ScoreComp->AddPoints(ScoreValue);
	
	ASkillSystemPlayerController* PlayerController =
		Cast<ASkillSystemPlayerController>(Player->GetController());
	if (!PlayerController)
	{
		return;
	}
	
	PlayerController->ShowEndGameScreen(ScoreComp->GetScore());
	
	PlayPickupSound();
	Destroy();
}

void ABigPoint::PlayPickupSound() const
{
	if (!PickupSound)
	{
		return;
	}

	UGameplayStatics::PlaySound2D(
		this,
		PickupSound);
}
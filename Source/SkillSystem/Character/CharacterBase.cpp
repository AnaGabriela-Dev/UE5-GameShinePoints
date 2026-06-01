// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterBase.h"
#include "SkillRelated/AbilityManagerComponent.h"
#include "Components/ResourceComponent.h"
#include "Components/HealthComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("Life"));
	ManaComp = CreateDefaultSubobject<UResourceComponent>(TEXT("Mana"));
	
	AbilityManager = CreateDefaultSubobject<UAbilityManagerComponent>(
		TEXT("AbilityManager"));

	FireballSpawnPoint = CreateDefaultSubobject<USceneComponent>(
		TEXT("FireballSpawnPoint"));
	FireballSpawnPoint->SetupAttachment(RootComponent);
	
	// COLLISIONS
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->InitCapsuleSize(34.f, 88.f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
	HealthComp->OnDeath.AddDynamic(this, &ACharacterBase::HandleDeath);
}

void ACharacterBase::HandleDeath()
{
    PlayDeathAnimation();
	
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->DisableMovement();
        GetCharacterMovement()->StopMovementImmediately();
    }
}

void ACharacterBase::PlayMontage(UAnimMontage* Montage)
{
	if (!Montage)
	{
		return;
	}

	if (!GetMesh())
	{
		return;
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (!AnimInstance)
	{
		return;
	}

	AnimInstance->Montage_Play(Montage);
}

void ACharacterBase::PlayHitReaction()
{
	if (!HitReactionAnimMontage)
	{
		return;
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance)
	{
		return;
	}

	if (AnimInstance->Montage_IsPlaying(HitReactionAnimMontage))
	{
		return;
	}
	
	PlayMontage(HitReactionAnimMontage);
}

void ACharacterBase::PlayAbilityAnimation(UAnimMontage* Montage)
{
	if (!Montage)
	{
		return;
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (!AnimInstance)
	{
		return;
	}
	
	AnimInstance->Montage_Play(Montage);
}

void ACharacterBase::PlayRespawnAnimation()
{
	PlayMontage(RespawnAnimMontage);
}

void ACharacterBase::PlayDeathAnimation()
{
	PlayMontage(DeathAnimMontage);
}

ETeam ACharacterBase::GetTeam() const
{
	return Team;
}

UResourceComponent* ACharacterBase::GetResourceComponent() const
{
	return ManaComp;
}


USceneComponent* ACharacterBase::GetFireballSpawnPoint() const
{
	return FireballSpawnPoint;
}

FVector ACharacterBase::GetAbilityAimDirection() const
{
	return GetActorForwardVector();
}


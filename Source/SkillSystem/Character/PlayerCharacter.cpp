#include "Character/PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Components/HealthComponent.h"
#include "Components/ScoreComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SkillRelated/AbilityManagerComponent.h"
#include "System/SkillSystem.h"
#include "System/SkillSystemGameMode.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	Team = ETeam::Player;
	
	ScoreComponent = CreateDefaultSubobject<UScoreComponent>(
		TEXT("ScoreComponent"));
	
	// CHARACTER ROTATION
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = false;

	// MOVEMENT
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.f;
	GetCharacterMovement()->AirControl = 0.8f;
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->MaxWalkSpeed = 400.f;

	// CAMERA
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT
		("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 350.f;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	CameraBoom->SocketOffset = FVector(0.f, 50.f, 10.f);

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(
		TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, 
		USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	
	// FIREBALL SPAWN
	FireballSpawnPoint->SetupAttachment(GetMesh());
	FireballSpawnPoint->SetRelativeLocation(FVector(60.f, 0.f, 
		-10.f));
	
	// COLLISION
	GetCharacterMovement()->bEnablePhysicsInteraction = false;
	GetCharacterMovement()->PushForceFactor = 0.f;
}

void APlayerCharacter::SetupPlayerInputComponent(
	UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent =Cast
		<UEnhancedInputComponent>(PlayerInputComponent);

	if (!EnhancedInputComponent)
	{
		UE_LOG(LogSkillSystem, Error, TEXT("[SetupPlayerInputComponent] Failed "
									 "to find Enhanced Input Component"));

		return;
	}

	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered,
		this, &APlayerCharacter::Look);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, 
		this, &APlayerCharacter::Move);

	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, 
		this, &APlayerCharacter::Jump);

	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, 
		this, &APlayerCharacter::StopJumping);

	EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, 
		this, &APlayerCharacter::StartSprint);

	EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, 
		this, &APlayerCharacter::StopSprint);

	EnhancedInputComponent->BindAction(AbilityAction, ETriggerEvent::Started, 
		this, &APlayerCharacter::Input_Ability);
	
	EnhancedInputComponent->BindAction(AbilityAction_2, ETriggerEvent::Started,
		this, &APlayerCharacter::Input_Ability);
	
	EnhancedInputComponent->BindAction(QuitAction, ETriggerEvent::Started,
		this, &APlayerCharacter::QuitGame);
}

void APlayerCharacter::QuitGame(const FInputActionValue& Value)
{
	UKismetSystemLibrary::QuitGame(this,
		GetController<APlayerController>(), EQuitPreference::Quit,
		true);
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	if (!Controller)
	{
		return;
	}

	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(-LookAxisVector.Y);
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	if (!Controller)
	{
		return;
	}

	const FVector2D MovementVector = Value.Get<FVector2D>();
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw,0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis
		(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis
		(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

void APlayerCharacter::StartSprint(const FInputActionValue& Value)
{
	if (!GetCharacterMovement())
	{
		return;
	}

	GetCharacterMovement()->MaxWalkSpeed = 800.f;
}

void APlayerCharacter::StopSprint(const FInputActionValue& Value)
{
	if (!GetCharacterMovement())
	{
		return;
	}

	GetCharacterMovement()->MaxWalkSpeed = 400.f;
}

void APlayerCharacter::Input_Ability(const FInputActionInstance& Instance)
{
	if (!AbilityManager)
	{
		return;
	}

	const UInputAction* Action = Instance.GetSourceAction();

	AbilityManager->ProcessAbilityInput(Action);
}

void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!IsLocallyControlled())
	{
		return;
	}

	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (!PlayerController)
	{
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem
		<UEnhancedInputLocalPlayerSubsystem> (PlayerController->GetLocalPlayer());

	if (!Subsystem)
	{
		return;
	}

	Subsystem->AddMappingContext(DefaultMappingContext,0);
}

void APlayerCharacter::HandleDeath()
{
	HealthComp->OnDeath.RemoveDynamic(this, &APlayerCharacter::HandleDeath);
	Super::HandleDeath();
	
	AController* SavedController = GetController();
	APlayerController* PC = Cast<APlayerController>(SavedController);
	
	if (PC && RespawnCooldownWidget)
	{
		UUserWidget* Widget = CreateWidget<UUserWidget>(PC,
				RespawnCooldownWidget);

		Widget->AddToViewport();
	}
	
	if (SavedController)
	{
		SavedController->UnPossess();

		ASkillSystemGameMode* GameMode = GetWorld()->GetAuthGameMode
			<ASkillSystemGameMode>();

		if (GameMode)
		{
			GameMode->RequestRespawn(SavedController);
		}
	}

	SetLifeSpan(1.0f);
}

FVector APlayerCharacter::GetAbilityAimDirection() const
{
	if (!Controller)
	{
		return GetActorForwardVector();
	}

	FVector CameraLocation;
	FRotator CameraRotation;

	Controller->GetPlayerViewPoint(CameraLocation, CameraRotation);

	const FVector TraceEnd =CameraLocation + (CameraRotation.Vector() * 
		10000.f);

	FHitResult Hit;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	const bool bHit = GetWorld()->LineTraceSingleByChannel(
			Hit,
			CameraLocation,
			TraceEnd,
			ECC_Visibility,
			Params);

	const FVector TargetPoint = bHit ? Hit.ImpactPoint : TraceEnd;

	const FVector SpawnLocation = GetFireballSpawnPoint()->GetComponentLocation();

	return (TargetPoint - SpawnLocation).GetSafeNormal();
}

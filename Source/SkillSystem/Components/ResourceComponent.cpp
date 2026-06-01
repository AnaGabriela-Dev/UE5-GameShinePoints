// Fill out your copyright notice in the Description page of Project Settings.

#include "ResourceComponent.h"
#include "MVVM/VM_Resource.h"
#include "Blueprint/UserWidget.h"
#include "View/MVVMView.h"

UResourceComponent::UResourceComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UResourceComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentResource = MaxResource;
	
	UpdateViewModel();
}

void UResourceComponent::TickComponent(float DeltaTime, ELevelTick TickType, 
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (!bShowDebug)
	{
		return;
	}
	
	if (GEngine)
	{
		FString DebugMsg = FString::Printf(TEXT("[Resource]: %.0f / %.0f"), 
			CurrentResource, MaxResource);
		GEngine->AddOnScreenDebugMessage(1, 0.0f, FColor::Green, 
			DebugMsg);
	}
	
}

void UResourceComponent::RegenerateResource()
{
	if (!bEnableRegen || CurrentResource >= MaxResource)
	{
		return;
	}
	
	RestoreResource(RegenRate);
}

void UResourceComponent::StartRegenTimer(const float StartDelay, const float 
	TickDelay)
{
	if (!bEnableRegen)
	{
		return;
	}
	
	GetWorld()->GetTimerManager().SetTimer(RegenTimerHandle, this, 
		&UResourceComponent::RegenerateResource, TickDelay, true, 
		StartDelay);
}

void UResourceComponent::ConsumeResource(float Amount)
{
	if (!GetOwner() || !GetWorld())
	{
		return;
	}
	
	if (!HasEnoughResource(Amount))
	{
		return;
	}
	
	// Guarantee CurrentResource will always be between 0 and MaxResource
	CurrentResource = FMath::Clamp(CurrentResource - Amount, 0.0f, 
		MaxResource);
	if (ResourceVM)
	{
		ResourceVM->SetCurrentResource(CurrentResource);
	}
	
	if (!GetWorld()->GetTimerManager().IsTimerActive(RegenTimerHandle))
	{
		StartRegenTimer(RegenStartDelay, RegenTickDelay);
	}
}

void UResourceComponent::RestoreResource(const float Amount)
{
	if (!GetOwner() || !GetWorld())
	{
		return;
	}
	
	CurrentResource = FMath::Clamp(CurrentResource + Amount, 0.0f, 
		MaxResource);
	if (ResourceVM)
	{
		ResourceVM->SetCurrentResource(CurrentResource);
	}
	
	if (CurrentResource == MaxResource)
	{
		GetWorld()->GetTimerManager().ClearTimer(RegenTimerHandle);
	}
}

bool UResourceComponent::HasEnoughResource(const float ResourceCost) const
{
	if (CurrentResource < ResourceCost)
	{
		const FString OwnerName = GetOwner()->GetName();
		UE_LOG(LogTemp, Warning, TEXT("[Resource] %s Não tem resurso o "
								"suficiente (Cost: %.0f | Current: %.0f)"), 
								*OwnerName, ResourceCost, CurrentResource);
		
		return false;
	}

	return true;
}

void UResourceComponent::UpdateViewModel() const
{
	if (!ResourceVM)
	{
		return;
	}
	
	ResourceVM->SetMaxResource(MaxResource);
	ResourceVM->SetCurrentResource(CurrentResource);
}


void UResourceComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (!GetWorld())
	{
		return;
	}
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}

float UResourceComponent::GetMaxResource() const
{
	return MaxResource;
}

void UResourceComponent::SetCurrentResource(const float Amount)
{
	CurrentResource = Amount;
}

void UResourceComponent::SetViewModel(UVM_Resource* InResourceVM)
{
	ResourceVM = InResourceVM;
	bIsVMReady = true;
	UpdateViewModel();
}

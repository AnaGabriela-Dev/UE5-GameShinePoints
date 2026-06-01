#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sound/SoundBase.h"
#include "BigPoint.generated.h"

class USphereComponent;
class UStaticMeshComponent;

/** 
 * Represents a high-value collectible actor that awards bonus points to the 
 * player upon collision overlap.
 */
UCLASS()
class SKILLSYSTEM_API ABigPoint : public AActor
{
	GENERATED_BODY()

public:
	ABigPoint();

protected:
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
	
protected:
	void PlayPickupSound() const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Audio")
	TObjectPtr<USoundBase> PickupSound;
	
private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Collision = nullptr;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh = nullptr;

	UPROPERTY(EditAnywhere)
	int ScoreValue = 500;
	
private:
	bool bCollected = false;
};
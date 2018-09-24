// Copy right NestorColt 2018

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h" // always generated file has to be the last included

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGSCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();
	

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void OpenDoor();
	void CloseDoor();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	

private:

	AActor * Owner;

	UPROPERTY(EditAnywhere)
	float OpenAngle = 20.F;
	FRotator DefaultRotation = FRotator(0.f, 90.f, 0.f);

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate;

	//-
	float DoorCloseDelay = 0.25f;
	float LastDoorOpenTime;

	//  
	float MassOfActorsInPlate();

};

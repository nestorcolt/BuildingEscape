// Copy right NestorColt 2018
#pragma once

#include "OpenDoor.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"



// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	ActorThatOpenDoor =  GetWorld()->GetFirstPlayerController()->GetPawn();

	
}

void UOpenDoor::OpenDoor() {
	AActor* Owner = GetOwner();
	FRotator Rotation = FRotator(0.f, 30.f, 0.f);

	Owner->SetActorRotation(Rotation);
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ... this is check every frame in the fame. VERY, VERY POWERFULL !

	if (PressurePlate->IsOverlappingActor(ActorThatOpenDoor))
	{
		OpenDoor();
	}
}


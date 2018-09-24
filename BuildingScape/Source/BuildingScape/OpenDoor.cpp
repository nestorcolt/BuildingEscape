// Copy right NestorColt 2018
#pragma once

#include "OpenDoor.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Components/ActorComponent.h"
#include "Components/PrimitiveComponent.h"

#define OUT

// ------------------------------------------------------------------------------------------------------------
// 

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
	Owner = GetOwner();

	//Set default door rotation stage
	Owner->SetActorRotation(DefaultRotation);

	AActor* Player = GetWorld()->GetFirstPlayerController();
	UE_LOG(LogTemp, Warning, TEXT("Player: %s"), *Player->GetName());
	
}

void UOpenDoor::OpenDoor() 
{
	FRotator Rotation = FRotator(0.f, OpenAngle, 0.f);
	Owner->SetActorRotation(Rotation);
}

void UOpenDoor::CloseDoor() {
	FRotator Rotation = FRotator(DefaultRotation);
	Owner->SetActorRotation(Rotation);
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ... this is check every frame in the fame. VERY, VERY POWERFULL !

	if (MassOfActorsInPlate() > 70.f)
	{
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetRealTimeSeconds();
	}

	if (GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay)
	{
		CloseDoor();
	}
}

// ------------------------------------------------------------------------------------------------------------
// 


float UOpenDoor::MassOfActorsInPlate() 
{

	float TotalMass = 0.f;
	TArray<AActor*> OverlapingActors;
	PressurePlate->GetOverlappingActors(OUT OverlapingActors);

	for (const auto& actor : OverlapingActors)
	{
		TotalMass += actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT(" Actors: %s"), *actor->GetName());
	}

	return TotalMass;
}
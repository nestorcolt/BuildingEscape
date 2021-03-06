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

	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("PresurePlate component of %s not found"), *GetOwner()->GetName());
	}

	//Set default door rotation stage
	Owner->SetActorRotation(DefaultRotation);

	float PlayerMass = 0.f;
	AActor* Player = GetWorld()->GetFirstPlayerController()->GetPawn();
	PlayerMass = Player->FindComponentByClass<UPrimitiveComponent>()->GetMass();

	UE_LOG(LogTemp, Warning, TEXT("Player: %s"), *Player->GetName());
	UE_LOG(LogTemp, Warning, TEXT("PlayerMass: %d"), PlayerMass);

	
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ... this is check every frame in the fame. VERY, VERY POWERFULL !

	if (MassOfActorsInPlate() > TriggerMass)
	{
		OnOpenEvent.Broadcast();
	}

	else
	{
		OnCloseEvent.Broadcast();
	}
}

// ------------------------------------------------------------------------------------------------------------
// 


float UOpenDoor::MassOfActorsInPlate() 
{

	float TotalMass = 0.f;
	TArray<AActor*> OverlapingActors;
	if (!PressurePlate) { return TotalMass; }
	PressurePlate->GetOverlappingActors(OUT OverlapingActors);

	for (const auto& actor : OverlapingActors)
	{
		TotalMass += actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT(" Actors: %s"), *actor->GetName());
	}

	return TotalMass;
}


// Copy right NestorColt 2018

#include "Grabber.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for duty"));
	// ...

	///Look for attached physics handle
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{

	}
	else	
	{
		UE_LOG(LogTemp, Error, TEXT("Physics handle of %s not found"), *GetOwner()->GetName());
	}

	///Look for attached UInputComponent
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Input component found"));
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Input component of %s not found"), *GetOwner()->GetName());
	}
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	//get player view point this tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointLRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointLRotation
	);

	//LOG OUT to test
	//UE_LOG(LogTemp, Warning, TEXT("Location At: %s Rotation at: %s"), *PlayerViewPointLLocation.ToString(), *PlayerViewPointLRotation.ToString())

	//Create a Vector pointing up to the character position
	FVector LineTraceEnd = PlayerViewPointLocation + (PlayerViewPointLRotation.Vector() * Reach);

	//Draw a debug line
	DrawDebugLine(

		GetWorld(),
		PlayerViewPointLocation,
		LineTraceEnd,
		FColor(255, 0, 0),
		false,
		0.f,
		0.f,
		10.f
	);

	/// Setup query parameters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), true, GetOwner());

	/// Line trace:
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(

		OUT Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);


	//See what we hit
	AActor* ActorHit = Hit.GetActor();

	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor Hit (LineTrace - AKA RayCasting): %s"), *ActorHit->GetName())
	}

}

void UGrabber::Grab() 
{
	UE_LOG(LogTemp, Warning, TEXT("Grab method called with mouse or shift pressed"));
}

void UGrabber::Release() {
	UE_LOG(LogTemp, Warning, TEXT("Release method called with mouse or shift Released"));
}


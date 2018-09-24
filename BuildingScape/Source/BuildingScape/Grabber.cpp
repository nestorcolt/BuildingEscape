// Copy right NestorColt 2018

#include "Grabber.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"

#define OUT

/// ------------------------------------------------------------------------------------------------------------ 

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

/// ------------------------------------------------------------------------------------------------------------ 

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	// ...
	FindPhysicsHandleComponent();
	SetupInputComponent();
}

/// ------------------------------------------------------------------------------------------------------------ 

void UGrabber::SetupInputComponent() 

	{
	///Look for attached UInputComponent
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Input component of %s not found"), *GetOwner()->GetName());
	}
}

/// ------------------------------------------------------------------------------------------------------------ 

//Look for attached physics handle
void UGrabber::FindPhysicsHandleComponent()

	{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (PhysicsHandle)
	{

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Physics handle of %s not found"), *GetOwner()->GetName());
	}
}

/// ------------------------------------------------------------------------------------------------------------ 

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) 
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Create a Vector pointing up to the character position
	FVector LineTraceEnd = LineTrace("end");

	auto GrabbedComponent = PhysicsHandle->GetGrabbedComponent();

	if (GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
}

/// ------------------------------------------------------------------------------------------------------------ 

void UGrabber::Grab() 
{
	//Line Trace and see if we reach any actor with physics body collision channel set
	auto HitResult = GetPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();

	//if we hit something then attach it a physics handle
	if (ActorHit)
	{
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			ComponentToGrab,
			NAME_None,
			ActorHit->GetActorLocation(),
			ActorHit->GetActorRotation()
		);
	}

}

/// ------------------------------------------------------------------------------------------------------------ 

void UGrabber::Release() {
	PhysicsHandle->ReleaseComponent();
}

/// ------------------------------------------------------------------------------------------------------------ 

FVector UGrabber::LineTrace(FString Trace) {

	//get player view point this tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointLRotation;
	FVector LineTrace;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointLRotation
	);

	if (Trace == "end")
	{
		//Create a Vector pointing up to the character position
		 LineTrace = PlayerViewPointLocation + (PlayerViewPointLRotation.Vector() * Reach);
	}
	else if (Trace == "base")
	{
		 LineTrace = PlayerViewPointLocation;
	}

	return LineTrace;
}

/// ------------------------------------------------------------------------------------------------------------ 


FHitResult UGrabber::GetPhysicsBodyInReach() 
{

	// Get line traced from base to end
	FVector PlayerViewPointLocation = LineTrace("base");
	FVector LineTraceEnd = LineTrace("end");

	/// Setup query parameters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), true, GetOwner());

	/// Line trace:
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByObjectType(

		OUT HitResult,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);


	//See what we hit
	AActor* ActorHit = HitResult.GetActor();

	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor Hit (LineTrace - AKA RayCasting): %s"), *ActorHit->GetName())
	}
	return HitResult;
}

/// ------------------------------------------------------------------------------------------------------------ 

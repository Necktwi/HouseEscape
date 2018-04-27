// © FerryFair 2018

#include "Grabber.h"
#include <Public/DrawDebugHelpers.h>

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

	// ...
	
}


// Called every frame
void UGrabber::TickComponent (
   float DeltaTime, ELevelTick TickType,
   FActorComponentTickFunction* ThisTickFunction
) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
   FVector PlayerViewPointLocation;
   FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
      PlayerViewPointLocation, PlayerViewPointRotation
   );
   FVector LineTraceEnd = PlayerViewPointLocation + 
   (PlayerViewPointRotation.Vector() * Reach);
   DrawDebugLine(
      GetWorld(), PlayerViewPointLocation, LineTraceEnd, FColor(255, 0, 0),
      false, 0.f, 0.f,10.f
   );
   FHitResult OutHit;
   GetWorld()->LineTraceSingleByObjectType(OutHit, PlayerViewPointLocation,
      LineTraceEnd,
      FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
      FCollisionQueryParams(FName(TEXT("")), false, GetOwner())
   );
   AActor* ActorHit = OutHit.GetActor();
   if (ActorHit) {
      UE_LOG(LogTemp, Warning,
         TEXT("Line trace hit: %s"), *ActorHit->GetName()
      );
   }
}


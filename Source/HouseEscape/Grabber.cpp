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
   find_physics_handle_component();
   setup_input_component();
}

void UGrabber::find_physics_handle_component() {
   PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
   if (PhysicsHandle == nullptr) {
      UE_LOG(LogTemp, Error, TEXT("%s missing physics handle component"), 
         *GetOwner()->GetName()
      )
   }
}

void UGrabber::setup_input_component() {
   InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
   if (InputComponent) {
      InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::grab);
      InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::release);
   } else {
      UE_LOG(LogTemp, Error, TEXT("%s missing Input component"), 
         *GetOwner()->GetName()
      )
   }
}

// Called every frame
void UGrabber::TickComponent (
   float DeltaTime, ELevelTick TickType,
   FActorComponentTickFunction* ThisTickFunction
) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
   if (PhysicsHandle->GrabbedComponent) {
      PhysicsHandle->SetTargetLocation(get_reach_line_end());
   }
}

void UGrabber::grab() {
   UE_LOG(LogTemp, Warning, TEXT("Grab pressed"));
   auto HitResult = get_first_physics_body_in_reach();
   auto ComponentToGrab = HitResult.GetComponent();
   auto ActorHit = HitResult.GetActor();
   if (ActorHit) {
      PhysicsHandle->GrabComponentAtLocation(
         ComponentToGrab,
         NAME_None,
         ComponentToGrab->GetOwner()->GetActorLocation()
      );
   }
}

void UGrabber::release() {
   PhysicsHandle->ReleaseComponent();
}

const FHitResult UGrabber::get_first_physics_body_in_reach() {
   FHitResult OutHit;
   GetWorld()->LineTraceSingleByObjectType(
      OutHit,
      get_reach_line_start(),
      get_reach_line_end(),
      FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
      FCollisionQueryParams(FName(TEXT("")), false, GetOwner())
   );
   return OutHit;
}

FVector UGrabber::get_reach_line_start() {
   FVector PlayerViewPointLocation;
   FRotator PlayerViewPointRotation;
   GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
      PlayerViewPointLocation, PlayerViewPointRotation
   );
   return PlayerViewPointLocation;
}

FVector UGrabber::get_reach_line_end() {
   FVector PlayerViewPointLocation;
   FRotator PlayerViewPointRotation;
   GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
      PlayerViewPointLocation, PlayerViewPointRotation
   );
   return PlayerViewPointLocation + 
   (PlayerViewPointRotation.Vector() * Reach);
}

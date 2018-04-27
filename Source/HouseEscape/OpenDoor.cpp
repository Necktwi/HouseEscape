// © FerryFair 2018
                                                                                 
#include "OpenDoor.h"
#include "HouseEscape.h"
#include <GameFramework/Actor.h>
#include <Engine/World.h>
#include <Components/PrimitiveComponent.h>

// Sets default values for this component's properties
UOpenDoor::UOpenDoor () {
   // Set this component to be initialized when the game starts,
   // and to be ticked every frame.  You can turn these features
   // off to improve performance if you don't need them.
   PrimaryComponentTick.bCanEverTick = true;
   
}

// Called when the game starts
void UOpenDoor::BeginPlay () {
	Super::BeginPlay();
   ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
}

void UOpenDoor::open_door () {
   GetOwner()->SetActorRotation(FRotator(0.f, OpenAngle, 0.f));
}

void UOpenDoor::close_door() {
   GetOwner()->SetActorRotation(FRotator(0.f, 0.f, 0.f));
}

// Called every frame
void UOpenDoor::TickComponent (
   float DeltaTime, ELevelTick TickType,
   FActorComponentTickFunction* ThisTickFunction
) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
   if (DoorOpenFlag && 
      GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay
   ) {
      close_door();
      DoorOpenFlag = false;
   }
   if (PressurePlate->IsOverlappingActor(ActorThatOpens)) {
      open_door();
      DoorOpenFlag = true;
      LastDoorOpenTime = GetWorld()->GetTimeSeconds();
   }
}

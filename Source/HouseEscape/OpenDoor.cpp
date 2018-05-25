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
}

void UOpenDoor::open_door () {
   //GetOwner()->SetActorRotation(FRotator(0.f, OpenAngle, 0.f));
   OnOpen.Broadcast();
}

void UOpenDoor::close_door() {
   //GetOwner()->SetActorRotation(FRotator(0.f, 0.f, 0.f));
   OnClose.Broadcast();
}

// Called every frame
void UOpenDoor::TickComponent (
   float DeltaTime, ELevelTick TickType,
   FActorComponentTickFunction* ThisTickFunction
) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
   if (DoorOpenFlag && get_total_mass_of_actors_on_plate() < PPTriggerMass &&
      GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay
   ) {
      close_door();
      DoorOpenFlag = false;
   }
   if (get_total_mass_of_actors_on_plate() >= PPTriggerMass) {
      open_door();
      DoorOpenFlag = true;
      LastDoorOpenTime = GetWorld()->GetTimeSeconds();
   }
}

float UOpenDoor::get_total_mass_of_actors_on_plate() {
   float TotalMass = 0.f;
   TArray<AActor*> OverlappingActors;
   if (PressurePlate == nullptr) {
      UE_LOG(LogTemp, Error, TEXT("Missing PressurePlate."));
      return 0.0f;
   }
   PressurePlate->GetOverlappingActors(OverlappingActors);
   UE_LOG(LogTemp, Warning, TEXT("OverlappingActors count is %d"),
      OverlappingActors.Num()
   );
   for (auto& Actor : OverlappingActors) {
      TotalMass += 
      Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
   }
   UE_LOG(LogTemp, Warning, TEXT("Total mass is %f"), TotalMass);
   return TotalMass;
}

#include "Items/Treasures/Treasure.h"
#include "Characters/EchoCharacter.h"
#include "Kismet/GameplayStatics.h"

void ATreasure::OnSphereOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
    AEchoCharacter* EchoCharacter = Cast<AEchoCharacter>(OtherActor);
	if (EchoCharacter)
	{
        if (PickupSound)
        {
            UGameplayStatics::PlaySoundAtLocation(
                this,
                PickupSound,
                GetActorLocation()
            );
        }
        Destroy();
	}
}

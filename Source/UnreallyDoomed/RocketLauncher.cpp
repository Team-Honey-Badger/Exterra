

#include "UnreallyDoomed.h"
#include "RocketLauncher.h"



void ARocketLauncher::ProjectileFire()
{
	Super::ProjectileFire();

	if (ProjectileClass != NULL)
	{
		FVector MFLoc = WeaponMesh->GetSocketLocation("Weapon_Socket");
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, "MFLoc is " + MFLoc.ToString());
		FRotator MFRot = WeaponMesh->GetSocketRotation("Weapon_Socket");
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, "MFRot is " + MFRot.ToString());
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Instigator;
		ARocket const *Rocket = GetWorld()->SpawnActor<ARocket>(ProjectileClass, MFLoc, MFRot, SpawnParams);
		if (Rocket)
		{

		}
	}
}

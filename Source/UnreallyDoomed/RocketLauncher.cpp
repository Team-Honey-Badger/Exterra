

#include "UnreallyDoomed.h"
#include "RocketLauncher.h"



void ARocketLauncher::ProjectileFire()
{
	Super::ProjectileFire();

	if (ProjectileClass != NULL)
	{
		FVector MFLoc = WeaponMesh->GetSocketLocation("Weapon_Socket");
		FRotator MFRot = WeaponMesh->GetSocketRotation("Weapon_Socket");
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Instigator;
		ARocket const *Rocket = GetWorld()->SpawnActor<ARocket>(ProjectileClass, MFLoc, MFRot, SpawnParams);
		if (Rocket)
		{

		}
	}
}

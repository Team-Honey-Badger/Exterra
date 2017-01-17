

#include "UnreallyDoomed.h"
#include "Weapon.h"
#include "FPSCharacter.h"
#include "Engine.h"


// Sets default values
AWeapon::AWeapon()
{
 	 //Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true; //i may use this later but for now i'm commenting it out

	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComp"));
	RootComponent = CollisionComp;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->AttachTo(RootComponent);
}

//method that is called whenever the fire button is pressed
void AWeapon::Fire()
{
	if (ProjectileType == EWeaponProjectile::eBullet) {
		if (CurrentClip > 0)
		{
			InstantFire();
			PlayWeaponSound(FireSound);
			CurrentClip -= WeaponConfig.shotCost;
			if (MuzzleFX)
			{
				USkeletalMeshComponent* UseWeaponMesh = WeaponMesh;
				MuzzlePSC = UGameplayStatics::SpawnEmitterAttached(MuzzleFX, UseWeaponMesh, MuzzleAttachPoint);
			}
		}
		else
		{
			ReloadAmmo();
		}
	}
	if (ProjectileType == EWeaponProjectile::eSpread) {
		if (CurrentClip > 0)
		{
			for (int32 counter = 0; counter <= WeaponConfig.WeaponSpread; counter++) {
				InstantFire();
			}
			PlayWeaponSound(FireSound);
			CurrentClip -= WeaponConfig.shotCost;
			if (MuzzleFX)
			{
				USkeletalMeshComponent* UseWeaponMesh = WeaponMesh;
				MuzzlePSC = UGameplayStatics::SpawnEmitterAttached(MuzzleFX, UseWeaponMesh, MuzzleAttachPoint);
			}
		}
		else
		{
			ReloadAmmo();
		}
	}
	if (ProjectileType == EWeaponProjectile::eProjectile) {
		if (CurrentClip > 0)
		{
			ProjectileFire();
			PlayWeaponSound(FireSound);
			CurrentClip -= WeaponConfig.shotCost;
			if (MuzzleFX)
			{
				USkeletalMeshComponent* UseWeaponMesh = WeaponMesh;
				MuzzlePSC = UGameplayStatics::SpawnEmitterAttached(MuzzleFX, UseWeaponMesh, MuzzleAttachPoint);
			}
		}
		else
		{
			ReloadAmmo();
		}
	}
}

//sets the owner of the weapon to the player
void AWeapon::SetOwningPawn(AFPSCharacter *NewOwner)
{
	if (MyPawn != NewOwner)
	{
		Instigator = NewOwner;
		MyPawn = NewOwner;
	}
}

//method will attach the weapon to the player when called
void AWeapon::OnEquip()
{
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttachToPlayer();
}

//detaches the weapon from the player
void AWeapon::OnUnEquip()
{
	DetachFromPlayer();
}

//method that checks to see if the player exists and if it does then it will attach the weapon to the player
//if there is a weapon currently attached, then detach it, for the new weapon.
void AWeapon::AttachToPlayer()
{
	if (MyPawn)
	{
		DetachFromPlayer();

		USkeletalMeshComponent *Character = MyPawn->FirstPersonMesh;
		WeaponMesh->SetHiddenInGame(false);
		WeaponMesh->AttachTo(Character, "Weapon_Socket");
	}
}

//detaches the weapon from the character
void AWeapon::DetachFromPlayer()
{
	WeaponMesh->DetachFromParent();
	WeaponMesh->SetHiddenInGame(true);
}

/*
Method to reload ammo.
How it works:
if the current clip is 0, then subtract the max clip from the current ammo and put it in the current clip
if the current ammo is 0, then you are out of ammo
*/
void AWeapon::ReloadAmmo()
{
	if (CurrentAmmo > 0)
	{
		if (CurrentClip == 0)
		{
			if (CurrentAmmo < WeaponConfig.MaxClip)
			{
				CurrentClip = CurrentAmmo;
				CurrentAmmo = 0;
			}
			else
			{
				CurrentAmmo -= WeaponConfig.MaxClip;
				CurrentClip += WeaponConfig.MaxClip;
			}
		}
		else
		{
			if (CurrentAmmo < WeaponConfig.MaxClip)
			{
				CurrentClip = CurrentAmmo;
				CurrentAmmo = 0;
			}
			else
			{
				int32 ammoNeeded = WeaponConfig.MaxClip - CurrentClip;
				CurrentClip += ammoNeeded;
				CurrentAmmo -= ammoNeeded;
			}
		}
	}
}

//clear our the clip and the ammo of the weapon
void AWeapon::ClearAmmo()
{
	CurrentAmmo = 0;
	CurrentClip = 0;
}

//the instant, ray trace firing system, used for all non-projectile weapons.
void AWeapon::InstantFire()
{

		const int32 randomSeed = FMath::Rand();
		FRandomStream WeaponRandomStream(randomSeed);
		const float currentSpread = WeaponConfig.WeaponSpread;
		const float SpreadCone = FMath::DegreesToRadians(WeaponConfig.WeaponSpread * 0.5);
		const FVector AimDir = WeaponMesh->GetSocketRotation("MF").Vector();
		const FVector StartTrace = WeaponMesh->GetSocketLocation("MF");
		const FVector ShootDir = WeaponRandomStream.VRandCone(AimDir, SpreadCone, SpreadCone);
		const FVector EndTrace = StartTrace + ShootDir * WeaponConfig.WeaponRange;
		const FHitResult Impact = WeaponTrace(StartTrace, EndTrace);

		ProcessInstantHit(Impact, StartTrace, ShootDir, randomSeed, currentSpread);
}

void AWeapon::ProjectileFire()
{
	//nothing is going to be in here
	//calls the one in Rocket
}

//creates the ray trace for the instant fire
FHitResult AWeapon::WeaponTrace(const FVector &TraceFrom, const FVector &TraceTo) const
{
	static FName WeaponFireTag = FName(TEXT("WeaponTrace"));

	FCollisionQueryParams TraceParams(WeaponFireTag, true, Instigator);
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = true;
	TraceParams.AddIgnoredActor(this);

	FHitResult Hit(ForceInit);

	GetWorld()->LineTraceSingle(Hit, TraceFrom, TraceTo, TRACE_WEAPON, TraceParams);

	return Hit;
}

//checks to see if you hit anything, if it's a actor, then deal damage
void AWeapon::ProcessInstantHit(const FHitResult &Impact, const FVector &Origin, const FVector &ShootDir, int32 RandomSeed, float ReticleSpeed)
{
	const FVector EndTrace = Origin + ShootDir * WeaponConfig.WeaponRange;
	const FVector EndPoint = Impact.GetActor() ? Impact.ImpactPoint : EndTrace;

	// Deal damage to the hit actor
	if (Impact.GetActor()){
		Impact.GetActor()->TakeDamage(WeaponConfig.BulletDamageAmount, FDamageEvent(), GetInstigatorController(), this);
	}
}


// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeapon::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

UAudioComponent* AWeapon::PlayWeaponSound(USoundCue *Sound)
{
	UAudioComponent *AC = NULL;
	if (Sound && MyPawn)
	{
		AC = UGameplayStatics::PlaySoundAttached(Sound, MyPawn->GetRootComponent());
	}

	return AC;
}

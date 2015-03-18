

#include "UnreallyDoomed.h"
#include "Weapon.h"
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

void AWeapon::Fire()
{
	if (ProjectileType == EWeaponProjectile::eBullet) {
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, TEXT("Bullet"));
		InstantFire();
	}
	if (ProjectileType == EWeaponProjectile::eSpread) {
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, TEXT("Spread"));
		for (int32 counter = 0; counter <= WeaponConfig.WeaponSpread; counter++) {
			InstantFire();
		}
	}
	if (ProjectileType == EWeaponProjectile::eProjectile) {
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, TEXT("Projectile"));
		ProjectileFire();
	}
}

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

void AWeapon::ProcessInstantHit(const FHitResult &Impact, const FVector &Origin, const FVector &ShootDir, int32 RandomSeed, float ReticleSpeed)
{
	const FVector EndTrace = Origin + ShootDir * WeaponConfig.WeaponRange;
	const FVector EndPoint = Impact.GetActor() ? Impact.ImpactPoint : EndTrace;

	DrawDebugLine(this->GetWorld(), Origin, Impact.TraceEnd, FColor::Black, true, 10000, 3.f);

	AEnemy *Enemy = Cast<AEnemy>(Impact.GetActor());
	if (Enemy)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "YOU HIT AN ENEMY!!");
		Enemy->Destroy();
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


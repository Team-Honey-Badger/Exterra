

#include "UnreallyDoomed.h"
#include "Rocket.h"
#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
ARocket::ARocket()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<USphereComponent>("CollisionComp");
	CollisionComp->InitSphereRadius(50.0f);
	//CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this,&ARocket::OnCollision);
	RootComponent = CollisionComp;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileComp");
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 5000.f;
	ProjectileMovement->MaxSpeed = 5000.f;
	ProjectileMovement->bRotationFollowsVelocity = false;
	ProjectileMovement->bShouldBounce = false;

	InitialLifeSpan = 5.f;

}

// Called when the game starts or when spawned
void ARocket::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARocket::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ARocket::OnCollision(AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
	{
		//AEnemy *Enemy = Cast<AEnemy>(OtherActor);
		//if (Enemy)
		//{
		//	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "YOU BLEW UP AN ENEMY!!");
		//	Enemy->Destroy();
		//	Destroy();
		//}

		// Deal damage to the hit actor
		if (!OtherActor->GetActorClass()->GetName().Contains("FPSCharacter"))
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::White, "OtherActor is " + OtherActor->GetActorClass()->GetName());
			OtherActor->TakeDamage(/*WeaponConfig.BulletDamageAmount*/100, FDamageEvent(), GetInstigatorController(), this);
		}
	}
}

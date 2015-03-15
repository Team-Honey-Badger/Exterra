

#pragma once

#include "GameFramework/Actor.h"
#include "Enemy.h"
#include "Rocket.generated.h"

UCLASS()
class UNREALLYDOOMED_API ARocket : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARocket();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	USphereComponent *CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	UProjectileMovementComponent *ProjectileMovement;

	UFUNCTION()
	void OnCollision(AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);	
};

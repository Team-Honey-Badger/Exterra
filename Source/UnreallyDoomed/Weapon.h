

#pragma once

#include "GameFramework/Actor.h"
#include "Rocket.h"
#include "Enemy.h"
#include "Weapon.generated.h"

#define TRACE_WEAPON ECC_GameTraceChannel1

UENUM(BlueprintType)
namespace EWeaponProjectile{
	enum ProjectileType{
		eBullet		UMETA(DisplayName = "Bullet"),
		eSpread		UMETA(DisplayName = "Spread"),
		eProjectile UMETA(DisplayName = "Projectile"),
	};
}

USTRUCT()
struct FWeaponData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, Category = Ammo)
	int32 MaxAmmo;

	UPROPERTY(EditDefaultsOnly, Category = Config)
	float TimeBetweenShots;

	UPROPERTY(EditDefaultsOnly, Category = Ammo)
	int32 shotCost;

	UPROPERTY(EditDefaultsOnly, Category = Config)
	float WeaponRange;

	UPROPERTY(EditDefaultsOnly, Category = Config)
	float WeaponSpread;

	UPROPERTY(EditDefaultsOnly, Category = Config)
	FString Name;

	UPROPERTY(EditDefaultsOnly, Category = Config)
	float BulletDamageAmount; //Damage per bullet to actors
};

UCLASS()
class UNREALLYDOOMED_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;


	UFUNCTION()
	void Fire();

	UFUNCTION()
	void InstantFire();

	UFUNCTION()
	virtual void ProjectileFire();

	UPROPERTY(EditDefaultsOnly, Category = Config)
	FWeaponData WeaponConfig;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Config)
	TEnumAsByte<EWeaponProjectile::ProjectileType> ProjectileType;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision)
	UBoxComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Config)
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class ARocket> ProjectileClass;

protected:
	FHitResult WeaponTrace(const FVector &TraceFrom, const FVector &TraceTo) const;

	void ProcessInstantHit(const FHitResult &Impact, const FVector &Origin, const FVector &ShootDir, int32 RandomSeed, float ReticleSpeed);

};

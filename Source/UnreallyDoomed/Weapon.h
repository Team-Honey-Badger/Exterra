#pragma once

#include "GameFramework/Actor.h"
#include "Rocket.h"
#include "Enemy.h"
#include "Weapon.generated.h"

class AFPSCharacter; //forward declaration

#define TRACE_WEAPON ECC_GameTraceChannel1

UENUM(BlueprintType)
namespace EWeaponProjectile{
	enum ProjectileType{
		eBullet		UMETA(DisplayName = "Bullet"),
		eSpread		UMETA(DisplayName = "Spread"),
		eProjectile UMETA(DisplayName = "Projectile"),
	};
}

USTRUCT(Blueprintable)
struct FWeaponData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ammo)
	int32 MaxAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	int32 MaxClip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	float TimeBetweenShots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ammo)
	int32 shotCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	float WeaponRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	float WeaponSpread;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	float BulletDamageAmount; //Damage per bullet to actors

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	int32 Priority;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	UTexture2D* SplashArt;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	FWeaponData WeaponConfig;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	TEnumAsByte<EWeaponProjectile::ProjectileType> ProjectileType;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision)
	UBoxComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Config)
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
	TSubclassOf<class ARocket> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = Config)
	USoundCue *FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	int32 CurrentAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	int32 CurrentClip;

	void SetOwningPawn(AFPSCharacter *NewOwner);

	void AttachToPlayer();
	void DetachFromPlayer();

	void OnEquip();
	void OnUnEquip();
	
	void ReloadAmmo();

	UAudioComponent* PlayWeaponSound(USoundCue *Sound);

protected:
	FHitResult WeaponTrace(const FVector &TraceFrom, const FVector &TraceTo) const;

	void ProcessInstantHit(const FHitResult &Impact, const FVector &Origin, const FVector &ShootDir, int32 RandomSeed, float ReticleSpeed);

	AFPSCharacter *MyPawn;

};

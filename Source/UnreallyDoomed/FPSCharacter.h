// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Weapon.h"
#include "Pistol.h"
#include "Shotgun.h"
#include "RocketLauncher.h"
#include "AssaultRifle.h"
#include "SMG.h"
#include "GameFramework/Character.h"
#include "FPSCharacter.generated.h"

/**
 * 
 */
UCLASS()
class UNREALLYDOOMED_API AFPSCharacter : public ACharacter
{
	GENERATED_BODY()
	
public:
	//BP functions called in Cpp
	UFUNCTION(BlueprintImplementableEvent, Category = "Code Triggered")
		void extendedTick();
	//virtual void extendedTick_Implementation();

	// movement variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement) //can be edited in blueprints
		float walkSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float runSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float crouchSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float defaultJumpZVelocity;

	// stat meters
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
		float maxStamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
		float maxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
		float maxEnergy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
		float maxAmmo;

	//is player running
	UPROPERTY(BlueprintReadOnly)									//can't be edited with blueprints, only read
		bool isRunning;

	UPROPERTY(BlueprintReadOnly)
		bool isFiring;

	//stamina is required to be able to run
	UPROPERTY(BlueprintReadOnly)
		float stamina;

	//character health
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
		float health;

	//character energy
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
		float energy;

	//character ammo
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
		float ammo;

	//Camera's initial position
	UPROPERTY()
		FVector initialRelativeLoc;
	UPROPERTY()
		FVector walkingRelativeLoc;
	UPROPERTY()
		FVector runningRelativeLoc;
	UPROPERTY()
		FVector jumpingRelativeLoc;

	//sound cues
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SoundCues)
		USoundCue *jumpRunCue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SoundCues)
		USoundCue *jumpCue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SoundCues)
		USoundCue *footstepCue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SoundCues)
		USoundCue *deathCue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SoundCues)
		USoundCue *fallingCue;

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		USkeletalMeshComponent* FirstPersonMesh;

	// Constructor for AFPSCharacter
	AFPSCharacter(/*const FObjectInitializer& ObjectInitializer*/);

	//ticks over time to passively update player
	virtual void Tick(float DeltaTime) override;

	//sets jump flag when key is pressed
	UFUNCTION()
		void OnStartJump();
	//clears jump flag when key is released
	UFUNCTION()
		void OnStopJump();

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision)
		UBoxComponent* CollisionComp;

	// ragdoll the character
	UFUNCTION(BlueprintCallable, Category = "Pawn")
		void Kill();

	//Weapon Related Stuffs
	UFUNCTION()
	void FireWeapon();

	UFUNCTION()
	void stopFireWeapon();

	void EquipPistol();
	void EquipShotgun();
	void EquipRocketLauncher();
	void EquipAssaultRifle();
	void EquipSMG();

	UPROPERTY(EditDefaultsOnly, Category = DefaultInv)
	TSubclassOf<class AWeapon> WeaponSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Actor)
	AWeapon *CurrentWeapon;
	
	UFUNCTION()
	void OnCollision(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	TArray<class AWeapon*> Inventory;

	void ProcessWeaponPickup(AWeapon *Weapon);

	void NextWeapon();
	void PrevWeapon();
	void EquipWeapon(AWeapon *Weapon);
	void GiveDefaultWeapons();

	UFUNCTION(BlueprintCallable, Category = Event)
	virtual void BeginPlay() override;

protected:

	//setup inputs
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	//handles moving forward/backward
	UFUNCTION()
		void MoveForward(float Val);

	//handles strafing
	UFUNCTION()
		void MoveRight(float Val);
	
	//handles running
	UFUNCTION()
		void StartRunning();
	UFUNCTION()
		void StopRunning();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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

	// movement variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement) //can be edited in blueprints
		float walkSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float runSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float crouchSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float maxStamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float maxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float maxEnergy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float maxAmmo;

	//is player running
	UPROPERTY(BlueprintReadOnly)
		bool isRunning;

	//stamina is required to be able to run
	UPROPERTY(BlueprintReadOnly)
		float stamina;

	//character health
	UPROPERTY(BlueprintReadOnly)
		float health;

	//character energy
	UPROPERTY(BlueprintReadOnly)
		float energy;

	//character ammo
	UPROPERTY(BlueprintReadOnly)
		float ammo;

	// Constructor for AFPSCharacter
	AFPSCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

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

	///** Pawn mesh: 1st person view (arms; seen only by self) */
	//UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	//	USkeletalMeshComponent* FirstPersonMesh;

	////handles firing
	//UFUNCTION()
	//	void OnFire();

	///** Gun muzzle's offset from the camera location */
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	//	FVector MuzzleOffset;

	///** Projectile class to spawn */
	//UPROPERTY(EditDefaultsOnly, Category = Projectile)
	//	TSubclassOf<class AFPSProjectile> ProjectileClass;

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

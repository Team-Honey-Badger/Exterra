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
	// Constructor for AFPSCharacter
	AFPSCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

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
	
	UPROPERTY()
		bool isRunning;

	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	//handles moving forward/backward
	UFUNCTION()
		void MoveForward(float Val);
	////handles running
	//UFUNCTION()
	//	void Run(float Val);
	////handles walking
	//UFUNCTION()
	//	void Walk(float Val);

	//handles strafing
	UFUNCTION()
		void MoveRight(float Val);
	
	//UFUNCTION()
		void StartRunning();

	//UFUNCTION()
		void StopRunning();
};

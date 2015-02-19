// Fill out your copyright notice in the Description page of Project Settings.

#include "UnreallyDoomed.h"
#include "FPSCharacter.h"

AFPSCharacter::AFPSCharacter(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	// Create a CameraComponent 
	FirstPersonCameraComponent = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->AttachParent = GetCapsuleComponent();

	// Position the camera a bit above the eyes
	//FirstPersonCameraComponent->RelativeLocation = FVector(0, 0, 50.0f + BaseEyeHeight);

	// Allow the pawn to control rotation.
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	//enable ticking
	CanEverTick();
}

void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();

	//initialize vars
	isRunning = false;
	stamina = maxStamina; // maybe allow BP to set starting stamina
	initialRelativeLoc = FirstPersonCameraComponent->RelativeLocation + FVector(5, 0, 0);
	runningRelativeLoc = FirstPersonCameraComponent->RelativeLocation + FVector(25, 0, -30);
	jumpingRelativeLoc = FirstPersonCameraComponent->RelativeLocation + FVector(20, 0, -2);
	walkingRelativeLoc = FirstPersonCameraComponent->RelativeLocation + FVector(5, 0, 0);
	GetCharacterMovement()->JumpZVelocity = defaultJumpZVelocity;

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("First Person Character In Use"));
	}
}

void AFPSCharacter::Tick(float DeltaTime)
{
	if (GetVelocity() == FVector(0, 0, 0)){ //if standing still	
		FirstPersonCameraComponent->RelativeLocation = initialRelativeLoc;
	}
	else if (!GetCharacterMovement()->IsMovingOnGround()){
		FirstPersonCameraComponent->RelativeLocation = jumpingRelativeLoc;
	}
	if (!isRunning){
		if (stamina < maxStamina){
			stamina += GetWorld()->GetDeltaSeconds(); //recharge stamina
			if (stamina < 0){
				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Fatigue Fading"));
				}
			}
			else{
				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Charging"));
				}
			}
			if (stamina > maxStamina){
				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Restored Max Stamina"));
				}
			}
		}
	}
}

void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* InputComponent)
{
	// set up gameplay key bindings
	InputComponent->BindAxis("MoveForward", this, &AFPSCharacter::MoveForward); // call custom class to handle movement
	InputComponent->BindAxis("MoveRight", this, &AFPSCharacter::MoveRight);
	InputComponent->BindAxis("Turn", this, &AFPSCharacter::AddControllerYawInput); // the Character class defines the 2 needed functions already
	InputComponent->BindAxis("LookUp", this, &AFPSCharacter::AddControllerPitchInput);
	InputComponent->BindAction("Jump", IE_Pressed, this, &AFPSCharacter::OnStartJump);
	InputComponent->BindAction("Jump", IE_Released, this, &AFPSCharacter::OnStopJump);
	InputComponent->BindAction("Sprint", IE_Pressed, this, &AFPSCharacter::StartRunning);
	InputComponent->BindAction("Sprint", IE_Released, this, &AFPSCharacter::StopRunning);
	InputComponent->BindAction("Suicide", IE_Pressed, this, &AFPSCharacter::Kill);
	//InputComponent->BindAction("Fire", IE_Pressed, this, &AFPSCharacter::OnFire); // click to shoot
}

void AFPSCharacter::MoveForward(float Value)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("move call"));
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		FRotator Rotation = Controller->GetControlRotation();
		
		// Limit pitch when walking or falling so that you aren't moving into the ground
		if (GetCharacterMovement()->IsMovingOnGround() || GetCharacterMovement()->IsFalling())
		{
			Rotation.Pitch = 0.0f;
		}
		
		// add movement in that direction
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);
		
		// boost speed if running
		if (isRunning && stamina > 0){ //running
			if (GetCharacterMovement()->IsMovingOnGround()){ //if not jumping
				FirstPersonCameraComponent->RelativeLocation = runningRelativeLoc;
			}
			GetCharacterMovement()->MaxWalkSpeed = runSpeed;
			stamina -= GetWorld()->GetDeltaSeconds(); //drain stamina
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Draining"));
			}

			// player enters fatigue after running out of stamina
			if (stamina < 0){
				stamina = -maxStamina;
				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Entering Max Fatigue"));
				}
			}
		}
		else{ //walking
			if (GetCharacterMovement()->IsMovingOnGround()){
				FirstPersonCameraComponent->RelativeLocation = walkingRelativeLoc;
			}
			GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
		}

		AddMovementInput(Direction, Value);
		
	}
}

//void AFPSCharacter::Run(float Value){
//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("RUNNING"));
//	isRunning = true;
//}
//void AFPSCharacter::Walk(float Value){
//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("MOVING"));
//	MoveForward(Value);
//}

void AFPSCharacter::StartRunning()
{
	if (GetVelocity() != FVector(0, 0, 0)){ // can only run if already moving
		isRunning = true;
		//PlaySoundAtLocation()
	}
	/*if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("RUNNING"));*/
}

void AFPSCharacter::StopRunning()
{
	isRunning = false;
	/*if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("WALKING"));*/
}

void AFPSCharacter::Kill()
{
	GetCapsuleComponent()->SetSimulatePhysics(true);
	GetCapsuleComponent()->AddForce(FVector(-5000, 0, 0));
}

void AFPSCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AFPSCharacter::OnStartJump()
{
	bPressedJump = true;
}
void AFPSCharacter::OnStopJump()
{
	bPressedJump = false;
}

//void AFPSCharacter::OnFire()
//{
//	// try and fire a projectile
//	if (ProjectileClass != NULL)
//	{
//		// Get the camera transform
//		FVector CameraLoc;
//		FRotator CameraRot;
//		GetActorEyesViewPoint(CameraLoc, CameraRot);
//
//		// MuzzleOffset is in camera space, so transform it to world space before offsetting from the camera to find the final muzzle position
//		FVector const MuzzleLocation = CameraLoc + FTransform(CameraRot).TransformVector(MuzzleOffset);
//		FRotator MuzzleRotation = CameraRot;
//		MuzzleRotation.Pitch += 10.0f;          // skew the aim upwards a bit
//		UWorld* const World = GetWorld();
//
//		if (World)
//		{
//			FActorSpawnParameters SpawnParams;
//			SpawnParams.Owner = this;
//			SpawnParams.Instigator = Instigator;
//
//			// spawn the projectile at the muzzle
//			AFPSProjectile* const Projectile = World->SpawnActor<AFPSProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
//
//			if (Projectile)
//			{
//				// find launch direction
//				FVector const LaunchDir = MuzzleRotation.Vector();
//				Projectile->InitVelocity(LaunchDir);
//			}
//		}
//	}
//}



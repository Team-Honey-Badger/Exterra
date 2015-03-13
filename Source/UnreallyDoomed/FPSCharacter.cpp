// Fill out your copyright notice in the Description page of Project Settings.

#include "UnreallyDoomed.h"
#include "FPSCharacter.h"
#include "Weapon.h"

AFPSCharacter::AFPSCharacter(/*const FObjectInitializer& ObjectInitializer*/)
: Super(/*ObjectInitializer*/)
{
	CurrentWeapon = NULL;

	Inventory.SetNum(3, false);

	CollisionComp = CreateDefaultSubobject<UBoxComponent>("CollisionComp");
	CollisionComp->AttachTo(RootComponent);
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSCharacter::OnCollision);

	// Create a CameraComponent 
	FirstPersonCameraComponent = /*ObjectInitializer.*/CreateDefaultSubobject<UCameraComponent>(/*this, */TEXT("FirstPersonCamera"));
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
	//FActorSpawnParameters SpawnParams;
	//SpawnParams.Owner = this;
	//SpawnParams.Instigator = Instigator;
	//AWeapon *Spawner = GetWorld()->SpawnActor<AWeapon>(WeaponSpawn, SpawnParams);

	//initialize vars
	isRunning = false;
	stamina = maxStamina; // maybe allow BP to set starting stamina
	initialRelativeLoc = FirstPersonCameraComponent->RelativeLocation + FVector(0, 0, 0);
	runningRelativeLoc = FirstPersonCameraComponent->RelativeLocation + FVector(20, 0, -30);
	jumpingRelativeLoc = FirstPersonCameraComponent->RelativeLocation + FVector(20, 0, -2);
	walkingRelativeLoc = FirstPersonCameraComponent->RelativeLocation + FVector(0, 0, 0);
	GetCharacterMovement()->JumpZVelocity = defaultJumpZVelocity;

	//if (Spawner)
	//{
	//	Spawner->AttachRootComponentTo(Mesh, "WeaponSocket", EAttachLocation::SnapToTarget);
	//	CurrentWeapon = Spawner;
	//}

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("First Person Character In Use"));
	}
}

void AFPSCharacter::Tick(float DeltaTime)
{
	if (GetVelocity() == FVector(0, 0, 0)){ //if standing still	
		FirstPersonCameraComponent->RelativeLocation = initialRelativeLoc;
		//add idle sound here
	}
	else if (!GetCharacterMovement()->IsMovingOnGround()){
		FirstPersonCameraComponent->RelativeLocation = jumpingRelativeLoc;
	}
	//else{
	//	if (isRunning){ //running
	//		PlaySoundOnActor(footstepCue);
	//	}
	//	else{ //walking
	//		PlaySoundOnActor(footstepCue);
	//	}
	//}
	if (!isRunning){
		if (stamina < maxStamina){
			stamina += GetWorld()->GetDeltaSeconds(); //recharge stamina
			/*if (stamina < 0){
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
			}*/
			/*if (stamina > maxStamina){
				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Restored Max Stamina"));
				}
			}*/
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
	InputComponent->BindAction("Fire", IE_Pressed, this, &AFPSCharacter::FireWeapon);
	InputComponent->BindAction("Pistol",IE_Pressed, this, &AFPSCharacter::EquipPistol);
	InputComponent->BindAction("Shotgun", IE_Pressed, this, &AFPSCharacter::EquipShotgun);
	InputComponent->BindAction("Rocket Launcher", IE_Pressed, this, &AFPSCharacter::EquipRocketLauncher);
	InputComponent->BindAction("Suicide", IE_Pressed, this, &AFPSCharacter::Kill);
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
			/*if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Draining"));
			}*/

			// player enters fatigue after running out of stamina
			if (stamina < 0){
				stamina = -maxStamina;
				/*if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Entering Max Fatigue"));
				}*/
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


void AFPSCharacter::StartRunning()
{
	if (GetVelocity() != FVector(0, 0, 0)){ // can only run if already moving
		isRunning = true;
	}
}

void AFPSCharacter::StopRunning()
{
	isRunning = false;
	
}

void AFPSCharacter::Kill()
{
	GetCapsuleComponent()->SetSimulatePhysics(true);
	//GetCapsuleComponent()->AddForce(FVector(-5000, 0, 0));
	PlaySoundOnActor(deathCue); // death sound
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
	if (isRunning){ //run and jump
		PlaySoundOnActor(jumpRunCue); //player sound effect
		
	}
	else{ //just a jump
		PlaySoundOnActor(jumpCue);
	}
	bPressedJump = true;
}
void AFPSCharacter::OnStopJump()
{
	bPressedJump = false;
}

void AFPSCharacter::FireWeapon()
{
	if (CurrentWeapon != NULL)
	{
		CurrentWeapon->Fire();
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "No Weapon Eqipped Currently");
	}
}

void AFPSCharacter::OnCollision(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	APistol *Pistol = Cast<APistol>(OtherActor);
	if (Pistol)
	{
		Inventory[0] = Pistol->GetClass();
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, "I just picked up a" + Pistol->WeaponConfig.Name);
		Pistol->Destroy();
	}
	AShotgun *Shotgun = Cast<AShotgun>(OtherActor);
	if (Shotgun)
	{
		Inventory[1] = Shotgun->GetClass();
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, "I just picked up a" + Shotgun->WeaponConfig.Name);
		Shotgun->Destroy();
	}
	ARocketLauncher *RocketLauncher = Cast<ARocketLauncher>(OtherActor);
	if (RocketLauncher)
	{
		Inventory[2] = RocketLauncher->GetClass();
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, "I just picked up a" + RocketLauncher->WeaponConfig.Name);
		RocketLauncher->Destroy();
	}
}

void AFPSCharacter::EquipPistol()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = Instigator;
	AWeapon *Spawner = GetWorld()->SpawnActor<AWeapon>(Inventory[0], SpawnParams);
	if (Spawner)
	{
		if (CurrentWeapon != NULL)
		{
			for (int32 counter = 0; counter < 3; counter++)
			{
				if (Inventory[counter] != NULL && Inventory[counter]->GetDefaultObject<AWeapon>()->WeaponConfig.Name == CurrentWeapon->WeaponConfig.Name)
				{
					Inventory[counter] = NULL;
					Inventory[counter] = CurrentWeapon->GetClass();
					GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, "I Put " + CurrentWeapon->WeaponConfig.Name + " Away in Slot " + FString::FromInt(counter));
				}
			}
			CurrentWeapon->Destroy();
			Spawner->CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			Spawner->AttachRootComponentTo(GetMesh(), "WeaponSocket", EAttachLocation::SnapToTarget);
			CurrentWeapon = Spawner;
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, "My Current Weapon is " + CurrentWeapon->WeaponConfig.Name);
		}
		else
		{
			Spawner->CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			Spawner->AttachRootComponentTo(GetMesh(), "WeaponSocket", EAttachLocation::SnapToTarget);
			CurrentWeapon = Spawner;
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, "My Current Weapon is " + CurrentWeapon->WeaponConfig.Name);
		}
	}
	GEngine->AddOnScreenDebugMessage(-1, 2.f ,FColor::Red, CurrentWeapon->GetActorLocation().ToString());
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "Spawner WeaponMesh to Component " + Spawner->WeaponMesh->GetComponentTransform().ToString());

}

void AFPSCharacter::EquipShotgun()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = Instigator;
	AWeapon *Spawner = GetWorld()->SpawnActor<AWeapon>(Inventory[1], SpawnParams);
	if (Spawner)
	{
		if (CurrentWeapon != NULL)
		{
			for (int32 counter = 0; counter < 3; counter++)
			{
				if (Inventory[counter] != NULL && Inventory[counter]->GetDefaultObject<AWeapon>()->WeaponConfig.Name == CurrentWeapon->WeaponConfig.Name)
				{
					Inventory[counter] = NULL;
					Inventory[counter] = CurrentWeapon->GetClass();
					GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, "I Put " + CurrentWeapon->WeaponConfig.Name + " Away in Slot " + FString::FromInt(counter));
				}
			}
			CurrentWeapon->Destroy();
			Spawner->CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			Spawner->AttachRootComponentTo(GetMesh(), "WeaponSocket", EAttachLocation::SnapToTarget);
			CurrentWeapon = Spawner;
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, "My Current Weapon is " + CurrentWeapon->WeaponConfig.Name);
		}
		else
		{
			Spawner->CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			Spawner->AttachRootComponentTo(GetMesh(), "WeaponSocket", EAttachLocation::SnapToTarget);
			CurrentWeapon = Spawner;
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, "My Current Weapon is " + CurrentWeapon->WeaponConfig.Name);
		}
	}

}

void AFPSCharacter::EquipRocketLauncher()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = Instigator;
	AWeapon *Spawner = GetWorld()->SpawnActor<AWeapon>(Inventory[2], SpawnParams);
	if (Spawner)
	{
		if (CurrentWeapon != NULL)
		{
			for (int32 counter = 0; counter < 3; counter++)
			{
				if (Inventory[counter] != NULL && Inventory[counter]->GetDefaultObject<AWeapon>()->WeaponConfig.Name == CurrentWeapon->WeaponConfig.Name)
				{
					Inventory[counter] = NULL;
					Inventory[counter] = CurrentWeapon->GetClass();
					GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, "I Put " + CurrentWeapon->WeaponConfig.Name + " Away in Slot " + FString::FromInt(counter));
				}
			}
			CurrentWeapon->Destroy();
			Spawner->CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			Spawner->AttachRootComponentTo(GetMesh(), "WeaponSocket", EAttachLocation::SnapToTarget);
			CurrentWeapon = Spawner;
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, "My Current Weapon is " + CurrentWeapon->WeaponConfig.Name);
		}
		else
		{
			Spawner->CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			Spawner->AttachRootComponentTo(GetMesh(), "WeaponSocket", EAttachLocation::SnapToTarget);
			CurrentWeapon = Spawner;
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, "My Current Weapon is " + CurrentWeapon->WeaponConfig.Name);
		}
	}

}


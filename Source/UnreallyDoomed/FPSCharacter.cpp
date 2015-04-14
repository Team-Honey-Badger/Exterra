// Fill out your copyright notice in the Description page of Project Settings.

#include "UnreallyDoomed.h"
#include "FPSCharacter.h"
#include "Weapon.h"

AFPSCharacter::AFPSCharacter(/*const FObjectInitializer& ObjectInitializer*/)
: Super(/*ObjectInitializer*/)
{
	CurrentWeapon = NULL;
	WeaponSpawn = NULL;

	Inventory.SetNum(5, false);

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

	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	FirstPersonMesh->SetOnlyOwnerSee(true);         // only the owning player will see this mesh
	FirstPersonMesh->AttachParent = FirstPersonCameraComponent;
	FirstPersonMesh->bCastDynamicShadow = false;
	FirstPersonMesh->CastShadow = false;

	//makes it so the fps arms cannot see the tps model
	GetMesh()->SetOwnerNoSee(true); 

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

	//adjust camera (disabled atm because we swithced to a first person mesh)
	initialRelativeLoc = FirstPersonCameraComponent->RelativeLocation + FVector(0, 0, 0);
	runningRelativeLoc = initialRelativeLoc; //FirstPersonCameraComponent->RelativeLocation + FVector(20, 0, -30);
	jumpingRelativeLoc = initialRelativeLoc; //FirstPersonCameraComponent->RelativeLocation + FVector(20, 0, -2);
	walkingRelativeLoc = initialRelativeLoc; //FirstPersonCameraComponent->RelativeLocation + FVector(0, 0, 0);
	/*runningRelativeLoc = FirstPersonCameraComponent->RelativeLocation + FVector(20, 0, -30);
	jumpingRelativeLoc = FirstPersonCameraComponent->RelativeLocation + FVector(20, 0, -2);
	walkingRelativeLoc = FirstPersonCameraComponent->RelativeLocation + FVector(0, 0, 0);*/
	GetCharacterMovement()->JumpZVelocity = defaultJumpZVelocity;

	//if (Spawner)
	//{
	//	Spawner->AttachRootComponentTo(Mesh, "Weapon_Socket", EAttachLocation::SnapToTarget);
	//	CurrentWeapon = Spawner;
	//}

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("First Person Character In Use"));
	}

	//GiveDefaultWeapons();
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
	//		UGameplayStatics::PlaySoundAttached(footstepCue, GetMesh());
	//	}
	//	else{ //walking
	//		UGameplayStatics::PlaySoundAttached(footstepCue, GetMesh());
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
				}is
			}*/
		}
	}

	if (isFiring)
	{
		CurrentWeapon->Fire();
	}

	//let the BP tick too by calling it's tick function
	extendedTick();
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
	InputComponent->BindAction("Fire", IE_Released, this, &AFPSCharacter::stopFireWeapon);
	InputComponent->BindAction("Pistol",IE_Pressed, this, &AFPSCharacter::EquipPistol);
	InputComponent->BindAction("Shotgun", IE_Pressed, this, &AFPSCharacter::EquipShotgun);
	InputComponent->BindAction("Rocket Launcher", IE_Pressed, this, &AFPSCharacter::EquipRocketLauncher);
	InputComponent->BindAction("Assault Rifle", IE_Pressed, this, &AFPSCharacter::EquipAssaultRifle);
	InputComponent->BindAction("Submachine Gun", IE_Pressed, this, &AFPSCharacter::EquipSMG);
	InputComponent->BindAction("NextWeapon", IE_Pressed, this, &AFPSCharacter::NextWeapon);
	InputComponent->BindAction("PrevWeapon", IE_Pressed, this, &AFPSCharacter::PrevWeapon);
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
			PawnMakeNoise(1.0f, GetActorLocation(RootComponent)); //alert AI of the noise
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
	//GetCapsuleComponent()->SetSimulatePhysics(true);
	//GetCapsuleComponent()->AddForce(FVector(-5000, 0, 0));
	UGameplayStatics::PlaySoundAttached(deathCue, GetMesh()); // death sound
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
		//UGameplayStatics::PlaySoundAttached(jumpRunCue, GetMesh()); //player sound effect
		if (!GetCharacterMovement()->IsFalling())
			UGameplayStatics::PlaySoundAttached(jumpRunCue, GetMesh()); //player sound effect
	}
	else{ //just a jump
		//UGameplayStatics::PlaySoundAttached(jumpCue, GetMesh());
		if (!GetCharacterMovement()->IsFalling())
			UGameplayStatics::PlaySoundAttached(jumpCue, GetMesh());
	}
	bPressedJump = true;
	PawnMakeNoise(1.0f, GetActorLocation(RootComponent)); //alert AI of the noise
}
void AFPSCharacter::OnStopJump()
{
	bPressedJump = false;
}

void AFPSCharacter::FireWeapon()
{
	if (CurrentWeapon != NULL)
	{
		if ((CurrentWeapon->WeaponConfig.Name == "Assault Rifle") || (CurrentWeapon->WeaponConfig.Name == "SMG"))
		{
			isFiring = true;
			//CurrentWeapon->Fire();
			PawnMakeNoise(1.0f, GetActorLocation(RootComponent)); //alert AI of the noise
		}
		else
		{
			CurrentWeapon->Fire();
			PawnMakeNoise(1.0f, GetActorLocation(RootComponent)); //alert AI of the noise
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "No Weapon Eqipped Currently");
	}
}

void AFPSCharacter::stopFireWeapon()
{
	isFiring = false;
}

//void AFPSCharacter::GiveDefaultWeapons()
//{
//	AWeapon *Spawner = GetWorld()->SpawnActor<AWeapon>(WeaponSpawn);
//	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, "Weapon Spawn is" + Spawner->WeaponConfig.Name);
//	//if (Spawner)
//	//{
//	//	Inventory[Spawner->WeaponConfig.Priority] = Spawner;
//	//	CurrentWeapon = Inventory[Spawner->WeaponConfig.Priority];
//	//	CurrentWeapon->SetOwningPawn(this);
//	//	CurrentWeapon->OnEquip();
//	//}
//}

void AFPSCharacter::OnCollision(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	AWeapon *Weapon = Cast<AWeapon>(OtherActor);
	if (Weapon)
	{
		ProcessWeaponPickup(Weapon);
	}
}

void AFPSCharacter::ProcessWeaponPickup(AWeapon *Weapon)
{
	if (Weapon != NULL)
	{
		if (Inventory[Weapon->WeaponConfig.Priority] == NULL)
		{
			AWeapon *Spawner = GetWorld()->SpawnActor<AWeapon>(Weapon->GetClass());
			if (Spawner)
			{
				Inventory[Spawner->WeaponConfig.Priority] = Spawner;
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, "You just picked up a" + Inventory[Spawner->WeaponConfig.Priority]->WeaponConfig.Name);
			}
			Weapon->Destroy();
		}
		else
		{
			if ((Inventory[Weapon->WeaponConfig.Priority]->CurrentAmmo >= 0) && (Weapon->CurrentAmmo <= Inventory[Weapon->WeaponConfig.Priority]->WeaponConfig.MaxAmmo - Inventory[Weapon->WeaponConfig.Priority]->CurrentAmmo))
			{
				Inventory[Weapon->WeaponConfig.Priority]->CurrentAmmo += Weapon->CurrentAmmo;
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "Added: " + Weapon->GetName() + Weapon->CurrentAmmo);
				Weapon->Destroy();
			}
			else
			{
				if (Inventory[Weapon->WeaponConfig.Priority]->CurrentAmmo > Inventory[Weapon->WeaponConfig.Priority]->WeaponConfig.MaxAmmo)
				{
					Inventory[Weapon->WeaponConfig.Priority]->CurrentAmmo = Inventory[Weapon->WeaponConfig.Priority]->WeaponConfig.MaxAmmo;
				}
			}
		}
	}
}

void AFPSCharacter::NextWeapon()
{
	if (CurrentWeapon != NULL)
	{
		if (Inventory[CurrentWeapon->WeaponConfig.Priority]->WeaponConfig.Priority != Inventory.Num() - 1)
		{
			if (Inventory[CurrentWeapon->WeaponConfig.Priority + 1] == NULL)
			{
				for (int32 counter = CurrentWeapon->WeaponConfig.Priority + 1; counter < Inventory.Num(); counter++)
				{
					if (Inventory[counter] && Inventory[counter]->IsA(AWeapon::StaticClass()))
					{
						EquipWeapon(Inventory[counter]);
					}
				}
			}
			else
			{
				EquipWeapon(Inventory[CurrentWeapon->WeaponConfig.Priority + 1]);
			}
		}
		else
		{
			EquipWeapon(Inventory[CurrentWeapon->WeaponConfig.Priority]);
		}
	}
	else
	{
		
	}
}

void AFPSCharacter::PrevWeapon()
{
	if (CurrentWeapon != NULL)
	{
		if (Inventory[CurrentWeapon->WeaponConfig.Priority]->WeaponConfig.Priority != 0)
		{
			if (Inventory[CurrentWeapon->WeaponConfig.Priority - 1] == NULL)
			{
				for (int32 counter = CurrentWeapon->WeaponConfig.Priority - 1; counter >= 0; counter--)
				{
					if (Inventory[counter] && Inventory[counter]->IsA(AWeapon::StaticClass()))
					{
						EquipWeapon(Inventory[counter]);
					}
				}
			}
			else
			{
				EquipWeapon(Inventory[CurrentWeapon->WeaponConfig.Priority - 1]);
			}
		}
		else
		{
			EquipWeapon(Inventory[CurrentWeapon->WeaponConfig.Priority]);
		}
	}
	else
	{

	}
}

void AFPSCharacter::EquipWeapon(AWeapon *Weapon)
{
	if (CurrentWeapon != NULL)
	{
		CurrentWeapon = Inventory[CurrentWeapon->WeaponConfig.Priority];
		CurrentWeapon->OnUnEquip();
		CurrentWeapon = Weapon;
		Weapon->SetOwningPawn(this);
		Weapon->OnEquip();
	}
	else
	{
		CurrentWeapon = Weapon;
		CurrentWeapon = Inventory[CurrentWeapon->WeaponConfig.Priority];
		CurrentWeapon->SetOwningPawn(this);
		Weapon->OnEquip();
	}
}

void AFPSCharacter::EquipPistol()
{
	if (Inventory[0] != NULL)
	{
		EquipWeapon(Inventory[0]);
	}
}

void AFPSCharacter::EquipSMG()
{
	if (Inventory[1] != NULL)
	{
		EquipWeapon(Inventory[1]);
	}
}

void AFPSCharacter::EquipShotgun()
{
	if (Inventory[2] != NULL)
	{
		EquipWeapon(Inventory[2]);
	}
}

void AFPSCharacter::EquipAssaultRifle()
{
	if (Inventory[3] != NULL)
	{
		EquipWeapon(Inventory[3]);
	}
}

void AFPSCharacter::EquipRocketLauncher()
{
	if (Inventory[4] != NULL)
	{
		EquipWeapon(Inventory[4]);
	}
}


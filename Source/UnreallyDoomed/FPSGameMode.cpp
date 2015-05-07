// Fill out your copyright notice in the Description page of Project Settings.

#include "UnreallyDoomed.h"
#include "FPSGameMode.h"

AFPSGameMode::AFPSGameMode(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	//DefaultPawnClass = AFPSCharacter::StaticClass(); // This tells the GameMode which type of Pawn to spawn for the player when starting the game. (replaced with blueprint)

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FObjectFinder<UBlueprint> PlayerPawnObject(TEXT("Blueprint'/Game/Blueprints/FPSCharacter_BP.FPSCharacter_BP'"));
	if (PlayerPawnObject.Object != NULL)
	{
		DefaultPawnClass = (UClass*)PlayerPawnObject.Object->GeneratedClass;
	}

	// set default HUD class to our Blueprinted HUD
	static ConstructorHelpers::FObjectFinder<UBlueprint> HUDObject(TEXT("Blueprint'/Game/UMG/MyHUD.MyHUD'"));
	if (HUDObject.Object != NULL)
	{
		HUDClass = (UClass*)HUDObject.Object->GeneratedClass;
	}
	//HUDClass = AFPSHUD::StaticClass();
}

void AFPSGameMode::StartPlay() // runs when game first starts
{
	Super::StartPlay();

	StartMatch();

	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Launching FPS Game Mode")); // print hello world to screen
	//}
}



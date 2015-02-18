// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "FPSGameMode.generated.h"

/**
 * 
 */
UCLASS()
class UNREALLYDOOMED_API AFPSGameMode : public AGameMode
{
	GENERATED_BODY()
	
	AFPSGameMode(const class FObjectInitializer& ObjectInitializer); // constructor

	virtual void StartPlay() override; // called when game starts
	
	
};

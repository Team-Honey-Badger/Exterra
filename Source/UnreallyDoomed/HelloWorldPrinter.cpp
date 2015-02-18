// Fill out your copyright notice in the Description page of Project Settings.

#include "UnreallyDoomed.h"
#include "HelloWorldPrinter.h"

AHelloWorldPrinter::AHelloWorldPrinter(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	MyNumber = 12;

}

void AHelloWorldPrinter::BeginPlay()
{
	Super::BeginPlay();

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Hello World!"));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::FromInt(MyNumber));
	}

}



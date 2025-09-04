// Fill out your copyright notice in the Description page of Project Settings.


#include "Crunch/Public/Players/CrunchCharacter.h"

// Sets default values
ACrunchCharacter::ACrunchCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACrunchCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACrunchCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACrunchCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


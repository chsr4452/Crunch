// Fill out your copyright notice in the Description page of Project Settings.


#include "Players/CrunchPlayerController.h"

#include "Players/CrunchPlayerCharacter.h"

void ACrunchPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	PlayerCharacter = Cast<ACrunchPlayerCharacter>(InPawn);
	if (PlayerCharacter)
	{
		PlayerCharacter->InitAbilityActorInfoOnServer();
	}
}

void ACrunchPlayerController::AcknowledgePossession(APawn* InPawn)
{
	Super::AcknowledgePossession(InPawn);
	if (PlayerCharacter)
	{
		PlayerCharacter->InitAbilityActorInfoOnClient();
	}
}

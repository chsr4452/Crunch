// Fill out your copyright notice in the Description page of Project Settings.


#include "Players/CrunchPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "Players/CrunchPlayerCharacter.h"
#include "UIs/GameplayWidget.h"

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
	PlayerCharacter = Cast<ACrunchPlayerCharacter>(InPawn);
	if (PlayerCharacter)
	{
		PlayerCharacter->InitAbilityActorInfoOnClient();
		CreateGameplayWidget();
	}
}

void ACrunchPlayerController::CreateGameplayWidget()
{
	if (!IsLocalPlayerController()) return;
	GameplayWidget = CreateWidget<UGameplayWidget>(this, GameplayWidgetClass);
	if (GameplayWidget)
	{
		GameplayWidget->AddToViewport();
	}
}

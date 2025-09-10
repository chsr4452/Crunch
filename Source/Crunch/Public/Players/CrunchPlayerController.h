// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CrunchPlayerController.generated.h"
class ACrunchPlayerCharacter;
/**
 * 
 */
UCLASS()
class CRUNCH_API ACrunchPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	
	//Server Side
	void OnPossess(APawn* InPawn) override;
	//Client Side
	void AcknowledgePossession(APawn* InPawn) override;
private:
	TObjectPtr<ACrunchPlayerCharacter> PlayerCharacter;
};

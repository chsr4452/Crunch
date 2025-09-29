// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GenericTeamAgentInterface.h"
#include "CrunchPlayerController.generated.h"

class UGameplayWidget;
class ACrunchPlayerCharacter;
/**
 * 
 */
UCLASS()
class CRUNCH_API ACrunchPlayerController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()
public:
	
	//Server Side
	void OnPossess(APawn* InPawn) override;
	//Client Side
	void AcknowledgePossession(APawn* InPawn) override;

	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
private:
	void CreateGameplayWidget();
	UPROPERTY()
	TObjectPtr<ACrunchPlayerCharacter> PlayerCharacter;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> GameplayWidgetClass;

	UPROPERTY()
	TObjectPtr<UGameplayWidget> GameplayWidget;

	UPROPERTY(Replicated)
	FGenericTeamId GenericTeamId;
};

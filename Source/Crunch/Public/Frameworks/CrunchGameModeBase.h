// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/GameModeBase.h"
#include "CrunchGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class CRUNCH_API ACrunchGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	virtual APlayerController* SpawnPlayerController(ENetRole InRemoteRole, const FString& Options) override;

private:
	FGenericTeamId GetTeamIDForPlayer(APlayerController* PlayerController);
	AActor* FindNextStartSpotForTeam(const FGenericTeamId& TeamID) const;

	UPROPERTY(EditDefaultsOnly, Category = "Team")
	TMap<FGenericTeamId, FName> TeamStartSpots;
};

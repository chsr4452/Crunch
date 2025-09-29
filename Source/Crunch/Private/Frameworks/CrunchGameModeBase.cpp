// Fill out your copyright notice in the Description page of Project Settings.


#include "Crunch/Public/Frameworks/CrunchGameModeBase.h"

#include "EngineUtils.h"
#include "GameFramework/PlayerStart.h"

APlayerController* ACrunchGameModeBase::SpawnPlayerController(ENetRole InRemoteRole, const FString& Options)
{
	APlayerController* NewPlayerController = Super::SpawnPlayerController(InRemoteRole, Options);
	IGenericTeamAgentInterface* NewPlayerTeamInterface = Cast<IGenericTeamAgentInterface>(NewPlayerController);
	FGenericTeamId NewTeamId = GetTeamIDForPlayer(NewPlayerController);
	
	if (NewPlayerTeamInterface)
	{
		NewPlayerTeamInterface->SetGenericTeamId(NewTeamId);
	}
	NewPlayerController->StartSpot = FindNextStartSpotForTeam(NewTeamId);
	return NewPlayerController;
}

FGenericTeamId ACrunchGameModeBase::GetTeamIDForPlayer(APlayerController* PlayerController)
{
	static int PlayerCount = 0;
	++PlayerCount;
	return FGenericTeamId(PlayerCount % 2);
}

AActor* ACrunchGameModeBase::FindNextStartSpotForTeam(const FGenericTeamId& TeamID) const
{
	const FName* StartSpotTag = TeamStartSpots.Find(TeamID);
	if (!StartSpotTag) return nullptr;
	UWorld* World = GetWorld();
	for (TActorIterator<APlayerStart> i(World); i; ++i)
	{
		if (i->PlayerStartTag == *StartSpotTag)
		{
			i->PlayerStartTag = FName("Taken");
			return *i;
		}
	}
	return nullptr;
}

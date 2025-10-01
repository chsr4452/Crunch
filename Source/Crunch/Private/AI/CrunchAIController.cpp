// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/CrunchAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

ACrunchAIController::ACrunchAIController()
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("AIPerceptionComponent");
	SenseConfig_Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("SenseConfig_Sight");
	SenseConfig_Sight->SightRadius = 1000.0f;
	SenseConfig_Sight->LoseSightRadius = 1200.f;
	SenseConfig_Sight->SetMaxAge(5.f);
	SenseConfig_Sight->PeripheralVisionAngleDegrees = 180.f;

	AIPerceptionComponent->ConfigureSense(*SenseConfig_Sight);
}

void ACrunchAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	SetGenericTeamId(FGenericTeamId(0));

	IGenericTeamAgentInterface* NewPlayerTeamInterface = Cast<IGenericTeamAgentInterface>(InPawn);
	if (NewPlayerTeamInterface)
	{
		NewPlayerTeamInterface->SetGenericTeamId(GetGenericTeamId());
	}
}

void ACrunchAIController::BeginPlay()
{
	Super::BeginPlay();
	RunBehaviorTree(BehaviorTree);
}

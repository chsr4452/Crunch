// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/CrunchAIController.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameplayAbilitySystems/CrunchAbilitySystemStatics.h"
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
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ACrunchAIController::OnTargetPerceptionUpdated);
	AIPerceptionComponent->OnTargetPerceptionForgotten.AddDynamic(this, &ACrunchAIController::OnTargetForgetton);
}

void ACrunchAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	SetGenericTeamId(FGenericTeamId(FMath::RandRange(0, 1)));

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

void ACrunchAIController::OnTargetPerceptionUpdated(AActor* InActor, FAIStimulus InStimulus)
{
	if (InStimulus.WasSuccessfullySensed())
	{
		if (!GetCurrentTarget())
		{
			SetCurrentTarget(InActor);
		}
	}
	else
	{
		ForgetActorIfDead(InActor);
	}
}

void ACrunchAIController::OnTargetForgetton(AActor* InActor)
{
	if (!InActor) return;
	if (GetCurrentTarget() == InActor) SetCurrentTarget(GetNextPerceiveActor());
}

const UObject* ACrunchAIController::GetCurrentTarget() const
{
	return GetBlackboardComponent()->GetValueAsObject(TargetBlackboardKeyName);
}

void ACrunchAIController::SetCurrentTarget(UObject* InTarget)
{
	if (InTarget)
	{
		GetBlackboardComponent()->SetValueAsObject(TargetBlackboardKeyName, InTarget);
	}
	else
	{
		GetBlackboardComponent()->ClearValue(TargetBlackboardKeyName);
	}
}

AActor* ACrunchAIController::GetNextPerceiveActor() const
{
	if (PerceptionComponent)
	{
		TArray<AActor*> PerceiveActors;
		AIPerceptionComponent->GetPerceivedHostileActors(PerceiveActors);
		if (PerceiveActors.Num() != 0)
		{
			return PerceiveActors[0];
		}
	}
	return nullptr;
}

void ACrunchAIController::ForgetActorIfDead(AActor* ActorToForget)
{
	const UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(ActorToForget);
	if (!ASC) return;

	if (ASC->HasMatchingGameplayTag(UCrunchAbilitySystemStatics::GetDeathStatusTag()))
	{
		for (UAIPerceptionComponent::TActorPerceptionContainer::TIterator i = AIPerceptionComponent->GetPerceptualDataIterator(); i; ++i)
		{
			if (i->Key != ActorToForget)
			{
				continue;
			}
			for (FAIStimulus& Stimuli : i->Value.LastSensedStimuli)
			{
				Stimuli.SetStimulusAge(TNumericLimits<float>::Max());
			}
		}
	}
}

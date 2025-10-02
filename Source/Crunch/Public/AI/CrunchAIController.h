// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CrunchAIController.generated.h"

struct FAIStimulus;
class UAISenseConfig_Sight;
/**
 * 
 */
UCLASS()
class CRUNCH_API ACrunchAIController : public AAIController
{
	GENERATED_BODY()

public:
	ACrunchAIController();
	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;
private:
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;
	
	UPROPERTY(VisibleDefaultsOnly, Category = "Perception")
	TObjectPtr<UAIPerceptionComponent> AIPerceptionComponent;
	
	UPROPERTY(VisibleDefaultsOnly, Category = "Perception")
	TObjectPtr<UAISenseConfig_Sight> SenseConfig_Sight;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName TargetBlackboardKeyName = "Target";

	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* InActor, FAIStimulus InStimulus);

	UFUNCTION()
	void OnTargetForgetton(AActor* InActor);

	const UObject* GetCurrentTarget() const;
	void SetCurrentTarget(UObject* InTarget);

	AActor* GetNextPerceiveActor() const;

	void ForgetActorIfDead(AActor* ActorToForget);
	
};

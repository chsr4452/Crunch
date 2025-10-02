// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTaskSendInputToASC.h"
#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

EBTNodeResult::Type UBTTaskSendInputToASC::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController)
	{
			UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(AIController->GetPawn());
		if (ASC)
		{
			ASC->PressInputID(static_cast<int32>(AbilityID));
			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystems/Abilities/GA_Combo.h"

#include "GameplayTagsManager.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "GameplayAbilitySystems/CrunchAbilitySystemStatics.h"

UGA_Combo::UGA_Combo()
{
	AbilityTags.AddTag(UCrunchAbilitySystemStatics::GetBasicAttackAbilityTag());
	BlockAbilitiesWithTag.AddTag(UCrunchAbilitySystemStatics::GetBasicAttackAbilityTag());
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_Combo::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo)) return;
	
	

	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		UAbilityTask_PlayMontageAndWait* PlayMontageAndWaitTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("Combo"), ComboMontage);
		PlayMontageAndWaitTask->OnBlendOut.AddDynamic(this, &UGA_Combo::K2_EndAbility);
		PlayMontageAndWaitTask->OnCancelled.AddDynamic(this, &UGA_Combo::K2_EndAbility);
		PlayMontageAndWaitTask->OnCompleted.AddDynamic(this, &UGA_Combo::K2_EndAbility);
		PlayMontageAndWaitTask->OnInterrupted.AddDynamic(this, &UGA_Combo::K2_EndAbility);
		PlayMontageAndWaitTask->ReadyForActivation();

		UAbilityTask_WaitGameplayEvent* WaitGameplayEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this,
			GetComboChangeEventTag(), nullptr, false, false);
		WaitGameplayEventTask->EventReceived.AddDynamic(this, &UGA_Combo::OnEventGameplayTaskReceive);
		WaitGameplayEventTask->ReadyForActivation();
	}

	UE_LOG(LogTemp, Warning, TEXT("Ability Activated!"))
}

FGameplayTag UGA_Combo::GetComboChangeEventTag()
{
	return FGameplayTag::RequestGameplayTag("Ability.Combo.Change");
}

FGameplayTag UGA_Combo::GetComboChangeEventTagEnd()
{
	return FGameplayTag::RequestGameplayTag("Ability.Combo.Change.End");
}

void UGA_Combo::OnEventGameplayTaskReceive(FGameplayEventData Data)
{
	FGameplayTag EventTag = Data.EventTag;

	if (EventTag == GetComboChangeEventTagEnd())
	{
		NextComboName = NAME_None;
		UE_LOG(LogTemp, Warning, TEXT("Combo Ended!"))
	}

	TArray<FName> TagNames;
	UGameplayTagsManager::Get().SplitGameplayTagFName(EventTag, TagNames);
	NextComboName = TagNames.Last();
	UE_LOG(LogTemp, Warning, TEXT("Next Combo: %s"), *NextComboName.ToString());
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystems/Abilities/GA_Combo.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayTagsManager.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_WaitInputPress.h"
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
		UAbilityTask_PlayMontageAndWait* PlayMontageAndWaitTask =
			UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
				this, TEXT("Combo"), ComboMontage);
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

	if (K2_HasAuthority())
	{
		UAbilityTask_WaitGameplayEvent* WaitDamageEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this,
			GetComboDamageEventTag(), nullptr, false, false);
		WaitDamageEventTask->EventReceived.AddDynamic(this, &UGA_Combo::OnEventDoDamage);
		WaitDamageEventTask->ReadyForActivation();
	}
	SetupWaitComboInputPress();
}

FGameplayTag UGA_Combo::GetComboChangeEventTag()
{
	return FGameplayTag::RequestGameplayTag("Ability.Combo.Change");
}

FGameplayTag UGA_Combo::GetComboChangeEventTagEnd()
{
	return FGameplayTag::RequestGameplayTag("Ability.Combo.Change.End");
}

FGameplayTag UGA_Combo::GetComboDamageEventTag()
{
	return FGameplayTag::RequestGameplayTag("Ability.Combo.Damage");
}

void UGA_Combo::SetupWaitComboInputPress()
{
	UAbilityTask_WaitInputPress* WaitInputPressTask = UAbilityTask_WaitInputPress::WaitInputPress(this);
	WaitInputPressTask->OnPress.AddDynamic(this, &UGA_Combo::HandleInputPress);
	WaitInputPressTask->ReadyForActivation();
}

void UGA_Combo::TryCommitCombo()
{
	if (NextComboName == NAME_None) return;
	if (UAnimInstance* OwnerAnimInstance = GetOwnerAnimInstance())
	{
		OwnerAnimInstance->Montage_SetNextSection(
			OwnerAnimInstance->Montage_GetCurrentSection(ComboMontage),
			NextComboName,
			ComboMontage
			);
	}
}

TSubclassOf<UGameplayEffect> UGA_Combo::GetDamageEffect() const
{
	if (UAnimInstance* OwnerAnimInstance = GetOwnerAnimInstance())
	{
		FName CurrentSection = OwnerAnimInstance->Montage_GetCurrentSection(ComboMontage);
		const TSubclassOf<UGameplayEffect>* Effect = DamageEffectMap.Find(CurrentSection);
		if (Effect)
		{
			return *Effect;
		}
	}
	return DamageEffectDefault;
}

void UGA_Combo::HandleInputPress(float TimeWaited)
{
	SetupWaitComboInputPress();
	TryCommitCombo();
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

void UGA_Combo::OnEventDoDamage(FGameplayEventData Data)
{
	UE_LOG(LogTemp, Warning, TEXT("OnEventDoDamage!"))
	TArray<FHitResult> HitResults = GetHitResultFromSweep(Data.TargetData, SphereSweepRadius, bDrawDebug, true);

	for (const FHitResult& HitResult : HitResults)
	{
		TSubclassOf<UGameplayEffect> Effect = GetDamageEffect();
		FGameplayEffectSpecHandle GameplayEffectSpecHandle = MakeOutgoingGameplayEffectSpec(Effect,
			GetAbilityLevel(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo()));

		FGameplayEffectContextHandle GameplayEffectContextHandle = MakeEffectContext(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo());
		GameplayEffectContextHandle.AddHitResult(HitResult);
		GameplayEffectContextHandle.AddInstigator(GetActorInfo().AvatarActor.Get(), GetActorInfo().AvatarActor.Get());
		GameplayEffectSpecHandle.Data->SetContext(GameplayEffectContextHandle);
		
		ApplyGameplayEffectSpecToTarget(GetCurrentAbilitySpecHandle(), CurrentActorInfo, CurrentActivationInfo,
			GameplayEffectSpecHandle, UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActor(HitResult.GetActor()));
	}
}

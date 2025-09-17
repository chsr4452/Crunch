// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystems/CrunchAbilitySystemComponent.h"

void UCrunchAbilitySystemComponent::InitGameplayEffects()
{
	if (!GetOwner() || !(GetOwner()->HasAuthority()))
	{
		return;
	}
	for (const TSubclassOf<UGameplayEffect>& Effect : InitialGameplayEffects)
	{
		FGameplayEffectContextHandle EffectContext = MakeEffectContext();
		FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingSpec(Effect, 1, EffectContext);
		ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
	}
}

void UCrunchAbilitySystemComponent::InitAbilities()
{
	if (!GetOwner() || !(GetOwner()->HasAuthority()))
	{
		return;
	}
	for (const TPair<ECrunchGameplayAbilityID, TSubclassOf<UGameplayAbility>> &Ability : BasicAbilities)
	{
		GiveAbility(FGameplayAbilitySpec(Ability.Value, 1, static_cast<int32>(Ability.Key)));
	}

	for (const auto &Ability : AdvanceAbilities)
	{
		GiveAbility(FGameplayAbilitySpec(Ability.Value, 0, static_cast<int32>(Ability.Key)));
	}
}

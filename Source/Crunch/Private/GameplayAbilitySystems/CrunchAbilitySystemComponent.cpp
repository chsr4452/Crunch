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

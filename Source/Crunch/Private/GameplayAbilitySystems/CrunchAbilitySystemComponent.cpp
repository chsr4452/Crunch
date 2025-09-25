// Fill out your copyright notice in the Description page of Project Settings.
#include "GameplayAbilitySystems/CrunchAbilitySystemComponent.h"
#include "GameplayAbilitySystems/CrunchAttributeSet.h"

UCrunchAbilitySystemComponent::UCrunchAbilitySystemComponent()
{
	GetGameplayAttributeValueChangeDelegate(UCrunchAttributeSet::GetHealthAttribute()).AddUObject(this, &UCrunchAbilitySystemComponent::HealthUpdate);
}

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

void UCrunchAbilitySystemComponent::HealthUpdate(const FOnAttributeChangeData& ChangedData)
{
	if (!GetOwner()) return;
	if (ChangedData.NewValue <= 0 && GetOwner()->HasAuthority() && DeathEffect)
	{
		AuthApplyGameplayEffectToSelf(DeathEffect);
	}
}

void UCrunchAbilitySystemComponent::AuthApplyGameplayEffectToSelf(TSubclassOf<UGameplayEffect> InEffect, int Level)
{
	if (GetOwner() && GetOwner()->HasAuthority())
	{
		FGameplayEffectContextHandle EffectContext = MakeEffectContext();
		FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingSpec(InEffect, Level, EffectContext);
		ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
	}
	
}

void UCrunchAbilitySystemComponent::ApplyFullStatEffect()
{
	if (!GetOwner()) return;
	AuthApplyGameplayEffectToSelf(FullStatEffect);
}

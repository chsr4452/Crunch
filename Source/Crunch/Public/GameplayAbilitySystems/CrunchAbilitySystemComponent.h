// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"

#include "Abilities/CrunchGameplayAbilityTypes.h"
#include "CrunchAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class CRUNCH_API UCrunchAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	UCrunchAbilitySystemComponent();
	void InitGameplayEffects();
	void InitAbilities();
	void ApplyFullStatEffect();
	
private:

	void HealthUpdate(const FOnAttributeChangeData& ChangedData);
	void AuthApplyGameplayEffectToSelf(TSubclassOf<UGameplayEffect> InEffect, int Level = 1);

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Effects")
	TSubclassOf<UGameplayEffect> FullStatEffect;
	
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Effects")
	TArray<TSubclassOf<UGameplayEffect>> InitialGameplayEffects;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Effects")
	TSubclassOf<UGameplayEffect> DeathEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Abilities")
	TMap<ECrunchGameplayAbilityID, TSubclassOf<UGameplayAbility>> BasicAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Abilities")
	TMap<ECrunchGameplayAbilityID, TSubclassOf<UGameplayAbility>> AdvanceAbilities;
};

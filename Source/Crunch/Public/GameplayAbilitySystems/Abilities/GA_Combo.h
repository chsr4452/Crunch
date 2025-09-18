// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySystems/CrunchGameplayAbilityBase.h"
#include "GA_Combo.generated.h"

/**
 * 
 */
UCLASS()
class CRUNCH_API UGA_Combo : public UCrunchGameplayAbilityBase
{
	GENERATED_BODY()
public:
	UGA_Combo();
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	FGameplayTag GetComboChangeEventTag();
	FGameplayTag GetComboChangeEventTagEnd();
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	TObjectPtr<UAnimMontage> ComboMontage;

	UFUNCTION()
	void OnEventGameplayTaskReceive(FGameplayEventData Data);

	FName NextComboName;
};

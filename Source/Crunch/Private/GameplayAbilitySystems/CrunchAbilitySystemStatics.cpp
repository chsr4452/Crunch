// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystems/CrunchAbilitySystemStatics.h"

#include "GameplayTagContainer.h"

FGameplayTag UCrunchAbilitySystemStatics::GetBasicAttackAbilityTag()
{
	return FGameplayTag::RequestGameplayTag("Ability.BasicAttack");
}

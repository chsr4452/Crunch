// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CrunchAbilitySystemStatics.generated.h"

struct FGameplayTag;
/**
 * 
 */
UCLASS()
class UCrunchAbilitySystemStatics : public UObject
{
	GENERATED_BODY()
public:
	static FGameplayTag GetBasicAttackAbilityTag();
	static FGameplayTag GetDeathStatusTag();
};

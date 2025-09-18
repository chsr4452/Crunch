// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "CrunchGameplayAbilityBase.generated.h"

/**
 * 
 */
UCLASS()
class CRUNCH_API UCrunchGameplayAbilityBase : public UGameplayAbility
{
	GENERATED_BODY()
protected:
	UAnimInstance* GetOwnerAnimInstance() const;
};

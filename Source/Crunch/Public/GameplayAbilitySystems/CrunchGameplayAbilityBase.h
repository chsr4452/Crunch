// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GenericTeamAgentInterface.h"
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
	TArray<FHitResult> GetHitResultFromSweep(const FGameplayAbilityTargetDataHandle& TargetDataHandle, float InSphereSweepRadius = 30.f,
		bool bInDrawDebug = false, bool bIgnoreSelf = true, ETeamAttitude::Type TargetTeam = ETeamAttitude::Hostile) const;

	UPROPERTY(EditDefaultsOnly, Category = "Debug")
	bool bDrawDebug = false;

	UPROPERTY(EditDefaultsOnly, Category = "Debug")
	float SphereSweepRadius = 30.f;
};

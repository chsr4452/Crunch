// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystems/CrunchGameplayAbilityBase.h"

UAnimInstance* UCrunchGameplayAbilityBase::GetOwnerAnimInstance() const
{
	USkeletalMeshComponent* OwnerSkeletalMesh = GetOwningComponentFromActorInfo();
	if (OwnerSkeletalMesh)
	{
		return OwnerSkeletalMesh->GetAnimInstance();
	}
	return nullptr;
}

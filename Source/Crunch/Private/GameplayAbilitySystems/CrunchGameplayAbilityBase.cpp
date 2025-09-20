// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitySystems/CrunchGameplayAbilityBase.h"
#include "Kismet/KismetSystemLibrary.h"

UAnimInstance* UCrunchGameplayAbilityBase::GetOwnerAnimInstance() const
{
	USkeletalMeshComponent* OwnerSkeletalMesh = GetOwningComponentFromActorInfo();
	if (OwnerSkeletalMesh)
	{
		return OwnerSkeletalMesh->GetAnimInstance();
	}
	return nullptr;
}

TArray<FHitResult> UCrunchGameplayAbilityBase::GetHitResultFromSweep(
	const FGameplayAbilityTargetDataHandle& TargetDataHandle, float SphereSweepRadius, bool bDrawDebug,
	bool bIgnoreSelf) const
{
	TArray<FHitResult> OutHitResults;
	TSet<AActor*> HitActors;
	
	for (const TSharedPtr<FGameplayAbilityTargetData> TargetData : TargetDataHandle.Data)
	{
		FVector StartLoc = TargetData->GetOrigin().GetTranslation();
		FVector EndLoc = TargetData->GetEndPoint();

		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

		TArray<AActor*> ActorsToIgnore;
		if (bIgnoreSelf)
		{
			ActorsToIgnore.Add(GetAvatarActorFromActorInfo());
		}
		EDrawDebugTrace::Type DrawDebugType = bDrawDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;

		TArray<FHitResult> Results;
		UKismetSystemLibrary::SphereTraceMultiForObjects(this, StartLoc, EndLoc, SphereSweepRadius, ObjectTypes,
			false, ActorsToIgnore, DrawDebugType, Results, true);

		for (const FHitResult& HitResult : Results)
		{
			if (HitActors.Contains(HitResult.GetActor()))
			{
				continue;
			}
			HitActors.Add(HitResult.GetActor());
			OutHitResults.Add(HitResult);
		}
	}
	
	return OutHitResults;
}

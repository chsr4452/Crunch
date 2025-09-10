// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CrunchAnimInstance.generated.h"

class UCharacterMovementComponent;
/**
 * 
 */
UCLASS()
class CRUNCH_API UCrunchAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	// the below functions are the native overrides for each phase
	// Native initialization override point
	virtual void NativeInitializeAnimation() override;
	
	// Native update override point. It is usually a good idea to simply gather data in this step and 
	// for the bulk of the work to be done in NativeThreadSafeUpdateAnimation.
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	// Native thread safe update override point. Executed on a worker thread just prior to graph update 
	// for linked anim instances, only called when the hosting node(s) are relevant
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe))
	FORCEINLINE float GetSpeed() const {return Speed;}

	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe))
	FORCEINLINE bool IsMoving() const {return Speed != 0;}

	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe))
	FORCEINLINE float GetYawSpeed() const {return YawSpeed;}

	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe))
	FORCEINLINE float GetSmoothYawSpeed() const {return SmoothedYawSpeed;}

	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe))
	FORCEINLINE bool IsFalling() const {return bFalling;}

	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe))
	FORCEINLINE float GetLookOffsetYaw() const {return LookOffsetRot.Yaw;}

	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe))
	FORCEINLINE float GetLookOffsetPitch() const {return LookOffsetRot.Pitch;}
	
private:
	
	UPROPERTY()
	TObjectPtr<ACharacter> OwnerCharacter;

	UPROPERTY()
	TObjectPtr<UCharacterMovementComponent> CharacterMovementComponent;

	float Speed;
	bool bFalling;

	FRotator BodyPrevRot;
	FRotator LookOffsetRot;
	
	float YawSpeed;
	float SmoothedYawSpeed;

	UPROPERTY(EditAnywhere, Category = "Animation")
	float SmoothLerpYawSpeed = 1.f;
};

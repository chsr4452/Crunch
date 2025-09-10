// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/CrunchAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UCrunchAnimInstance::NativeInitializeAnimation()
{
	if ((OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner())))
	{
		CharacterMovementComponent = OwnerCharacter->GetCharacterMovement();
		BodyPrevRot = OwnerCharacter->GetActorRotation();
	}
}

void UCrunchAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (CharacterMovementComponent)
	{
		Speed = CharacterMovementComponent->Velocity.Length();
		bFalling = CharacterMovementComponent->IsFalling();
		

		FRotator BodyRot = OwnerCharacter->GetActorRotation();
        FRotator BodyRotDelta = UKismetMathLibrary::NormalizedDeltaRotator(BodyRot, BodyPrevRot);
        BodyPrevRot = BodyRot;
        YawSpeed = BodyRotDelta.Yaw/ DeltaSeconds;
        SmoothedYawSpeed = UKismetMathLibrary::FInterpTo(SmoothedYawSpeed, YawSpeed, DeltaSeconds, SmoothLerpYawSpeed);
		FRotator AimRot = OwnerCharacter->GetBaseAimRotation();
		LookOffsetRot = UKismetMathLibrary::NormalizedDeltaRotator(AimRot, BodyRot);
			
		

	}
	
}

void UCrunchAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
}

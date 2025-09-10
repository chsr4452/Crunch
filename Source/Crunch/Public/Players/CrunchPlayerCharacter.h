// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Players/CrunchCharacter.h"
#include "CrunchPlayerCharacter.generated.h"


class UInputAction;
class UInputMappingContext;
class USpringArmComponent;
class UCameraComponent;

struct FInputActionValue;

/**
 * 
 */
UCLASS()
class CRUNCH_API ACrunchPlayerCharacter : public ACrunchCharacter
{
	GENERATED_BODY()
public:
	ACrunchPlayerCharacter();
	virtual void PawnClientRestart() override;


private:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	UPROPERTY(VisibleDefaultsOnly, Category = View)
	TObjectPtr<USpringArmComponent> SpringArmComponent;
	
	UPROPERTY(VisibleDefaultsOnly, Category = View)
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Jump;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Look;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Move;

	void LookInput(const FInputActionValue& InputActionValue);
	void MoveInput(const FInputActionValue& InputActionValue);

	FVector GetLookRightDir() const;
	FVector GetMoveFwdDir() const;
};

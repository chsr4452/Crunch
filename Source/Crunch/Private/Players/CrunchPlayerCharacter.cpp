// Fill out your copyright notice in the Description page of Project Settings.


#include "Crunch/Public/Players/CrunchPlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"

ACrunchPlayerCharacter::ACrunchPlayerCharacter()
{
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("Spring Arm Component");
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->bUsePawnControlRotation = true;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);

	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate.Yaw = 360.0f;
}

void ACrunchPlayerCharacter::PawnClientRestart()
{
	Super::PawnClientRestart();
	if (const APlayerController* PlayerController = GetController<APlayerController>())
	{
		UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem =
			PlayerController->GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
		if (EnhancedInputLocalPlayerSubsystem)
		{
			EnhancedInputLocalPlayerSubsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
}

void ACrunchPlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(GetController<APlayerController>()->InputComponent);
	EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Triggered, this, &ACrunchPlayerCharacter::Jump);
	EnhancedInputComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this, &ACrunchPlayerCharacter::LookInput);
	EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ACrunchPlayerCharacter::MoveInput);
}

void ACrunchPlayerCharacter::LookInput(const FInputActionValue& InputActionValue)
{
	FVector2D const LookVector = InputActionValue.Get<FVector2D>();
	AddControllerPitchInput(-(LookVector.Y));
	AddControllerYawInput(LookVector.X);
}

void ACrunchPlayerCharacter::MoveInput(const FInputActionValue& InputActionValue)
{
	FVector2D MoveVector = InputActionValue.Get<FVector2D>();
	MoveVector.Normalize();
	AddMovementInput(GetMoveFwdDir()*MoveVector.Y + GetLookRightDir()*MoveVector.X);
}

FVector ACrunchPlayerCharacter::GetLookRightDir() const
{
	return CameraComponent->GetRightVector();
}

FVector ACrunchPlayerCharacter::GetMoveFwdDir() const
{
	return FVector::CrossProduct(GetLookRightDir(), FVector::UpVector);
}

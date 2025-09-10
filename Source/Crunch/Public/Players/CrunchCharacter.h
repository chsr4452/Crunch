// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "CrunchCharacter.generated.h"

class UAttributeSet;

UCLASS()
class CRUNCH_API ACrunchCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACrunchCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/*******************************************************/
	/*					Gameplay Ability				   */
	/*******************************************************/
public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override {return CrunchAbilitySystemComponent;};
private:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> CrunchAbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<UAttributeSet> CrunchAttributeSet;

};

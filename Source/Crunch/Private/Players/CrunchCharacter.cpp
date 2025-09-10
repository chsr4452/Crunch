// Fill out your copyright notice in the Description page of Project Settings.


#include "Crunch/Public/Players/CrunchCharacter.h"
#include "AbilitySystemComponent.h"
#include "GameplayAbilitySystems/CrunchAbilitySystemComponent.h"
#include "GameplayAbilitySystems/CrunchAttributeSet.h"

// Sets default values
ACrunchCharacter::ACrunchCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	CrunchAbilitySystemComponent = CreateDefaultSubobject<UCrunchAbilitySystemComponent>("Ability System Component");
	CrunchAttributeSet = CreateDefaultSubobject<UCrunchAttributeSet>("Attribute Set");

}

void ACrunchCharacter::InitAbilityActorInfoOnServer()
{
	CrunchAbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UCrunchAbilitySystemComponent>(CrunchAbilitySystemComponent)->InitGameplayEffects();
}

void ACrunchCharacter::InitAbilityActorInfoOnClient()
{
	CrunchAbilitySystemComponent->InitAbilityActorInfo(this, this);
}

// Called when the game starts or when spawned
void ACrunchCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACrunchCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}




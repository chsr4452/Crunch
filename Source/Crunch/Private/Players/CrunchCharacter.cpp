// Fill out your copyright notice in the Description page of Project Settings.


#include "Crunch/Public/Players/CrunchCharacter.h"
#include "AbilitySystemComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "GameplayAbilitySystems/CrunchAbilitySystemComponent.h"
#include "GameplayAbilitySystems/CrunchAttributeSet.h"
#include "Kismet/GameplayStatics.h"
#include "UIs/OverheadAttrWidget.h"

// Sets default values
ACrunchCharacter::ACrunchCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	CrunchAbilitySystemComponent = CreateDefaultSubobject<UCrunchAbilitySystemComponent>("Ability System Component");
	CrunchAttributeSet = CreateDefaultSubobject<UCrunchAttributeSet>("Attribute Set");
	
	OverheadAttrBar = CreateDefaultSubobject<UWidgetComponent>("Overhead Attribute Bar");
	OverheadAttrBar->SetupAttachment(GetRootComponent());
	OverheadAttrBar->SetWidgetSpace(EWidgetSpace::Screen);
	OverheadAttrBar->SetDrawAtDesiredSize(true);
	OverheadAttrBar->SetOnlyOwnerSee(false);
	OverheadAttrBar->SetOwnerNoSee(false);
	
}

void ACrunchCharacter::InitAbilityActorInfoOnServer()
{
	CrunchAbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UCrunchAbilitySystemComponent>(CrunchAbilitySystemComponent)->InitGameplayEffects();
	Cast<UCrunchAbilitySystemComponent>(CrunchAbilitySystemComponent)->InitAbilities();
}

void ACrunchCharacter::InitAbilityActorInfoOnClient()
{
	CrunchAbilitySystemComponent->InitAbilityActorInfo(this, this);
}

void ACrunchCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if (NewController && !NewController->IsPlayerController())
	{
		InitAbilityActorInfoOnServer();
	}
}

// Called when the game starts or when spawned
void ACrunchCharacter::BeginPlay()
{
	Super::BeginPlay();
	ConfigOverheadAttrBar();
	
}

// Called every frame
void ACrunchCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ACrunchCharacter::IsLocallyControlledByPlayer() const
{
	return GetController() && GetController()->IsLocalPlayerController();
}

void ACrunchCharacter::ConfigOverheadAttrBar()
{
	if (!OverheadAttrBar) return;
	// Always pick the local viewport on this machine
	if (IsLocallyControlledByPlayer())
	{
		OverheadAttrBar->SetHiddenInGame(true);
		return;
	}
	UOverheadAttrWidget* OverheadAttrWidget = Cast<UOverheadAttrWidget>(OverheadAttrBar->GetUserWidgetObject());
	if (OverheadAttrWidget)
	{
		OverheadAttrWidget->ConfigASC(Cast<UCrunchAbilitySystemComponent>(GetAbilitySystemComponent()));
		OverheadAttrBar->SetHiddenInGame(false);
		GetWorldTimerManager().ClearTimer(OverheadAttrBarTimerHandle);
		GetWorldTimerManager().SetTimer(OverheadAttrBarTimerHandle, this, &ACrunchCharacter::UpdateOverheadVisibily, OverheadVisibilityPeriod, true);
	}
}

void ACrunchCharacter::UpdateOverheadVisibily()
{
	APawn* LocalPlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (LocalPlayerPawn)
	{
		float DistSquared = FVector::DistSquared(GetActorLocation(), LocalPlayerPawn->GetActorLocation());
		OverheadAttrBar->SetHiddenInGame(DistSquared > OverheadVisibilityRangeSquared);
	}
}




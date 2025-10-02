// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Crunch/Public/Players/CrunchCharacter.h"
#include "AbilitySystemComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameplayAbilitySystems/CrunchAbilitySystemComponent.h"
#include "GameplayAbilitySystems/CrunchAbilitySystemStatics.h"
#include "GameplayAbilitySystems/CrunchAttributeSet.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "UIs/OverheadAttrWidget.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"

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
	BindAbilitySystemDelegate();

	PerceptionStimuliSourceComponent = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>("Perception Stimuli Source");
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

void ACrunchCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACrunchCharacter, TeamID);
}

// Called when the game starts or when spawned
void ACrunchCharacter::BeginPlay()
{
	Super::BeginPlay();
	ConfigOverheadAttrBar();
	MeshInitRelativeTransform = GetMesh()->GetRelativeTransform();

	PerceptionStimuliSourceComponent->RegisterForSense(UAISense_Sight::StaticClass());
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

void ACrunchCharacter::BindAbilitySystemDelegate()
{
	if (CrunchAbilitySystemComponent)
	{
		CrunchAbilitySystemComponent->RegisterGameplayTagEvent(UCrunchAbilitySystemStatics::GetDeathStatusTag()).
		AddUObject(this, &ACrunchCharacter::DeathTagUpdated);
	}
}

void ACrunchCharacter::DeathTagUpdated(const FGameplayTag Tag, int32 NewCount)
{	UE_LOG(LogTemp, Warning, TEXT("DeathTagUpdated!"));
	if (NewCount != 0) StartDeath();
	else Respawn();
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

void ACrunchCharacter::PlayDeathMontage()
{
	if (DeathMontage)
	{
		float MontageDuration = PlayAnimMontage(DeathMontage);
		GetWorldTimerManager().SetTimer(DeathTimerHandle, this, &ACrunchCharacter::DeathMontageFinished, MontageDuration - 1.f);
	}
}

void ACrunchCharacter::SetStatusGaugeEnabled(bool bEnabled)
{
	if (bEnabled)
	{
		ConfigOverheadAttrBar();
	}
	else
	{
		OverheadAttrBar->SetHiddenInGame(true);
	}
}

void ACrunchCharacter::DeathMontageFinished()
{
	SetRagdollEnabled(true);
}

void ACrunchCharacter::SetRagdollEnabled(bool bEnabled)
{
	if (bEnabled)
	{
		GetMesh()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		GetMesh()->SetSimulatePhysics(true);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	}
	else
	{
		GetMesh()->SetSimulatePhysics(false);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMesh()->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		GetMesh()->SetRelativeTransform(MeshInitRelativeTransform);
	}
}

void ACrunchCharacter::StartDeath()
{
	OnDead();
	PlayDeathMontage();
	SetStatusGaugeEnabled(false);
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	GetCapsuleComponent()->SetCollisionEnabled((ECollisionEnabled::NoCollision));
	SetAIPerceptionStimuliSourceEnabled(false);
}

void ACrunchCharacter::Respawn()
{
	OnRespawn();
	SetRagdollEnabled(false);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	GetMesh()->GetAnimInstance()->StopAllMontages(0.f);
	SetStatusGaugeEnabled(true);
	if (HasAuthority() && GetController())
	{
		TWeakObjectPtr<AActor> StartSpot = GetController()->StartSpot;
		if (StartSpot.IsValid())
		{
			SetActorTransform(StartSpot->GetActorTransform());
		}
	}
	if (GetAbilitySystemComponent())
	{
		Cast<UCrunchAbilitySystemComponent>(GetAbilitySystemComponent())->ApplyFullStatEffect();
	}
	SetAIPerceptionStimuliSourceEnabled(true);
}

void ACrunchCharacter::OnDead()
{
}

void ACrunchCharacter::OnRespawn()
{
}

void ACrunchCharacter::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	TeamID = NewTeamID;
}

FGenericTeamId ACrunchCharacter::GetGenericTeamId() const
{
	return TeamID;
}

void ACrunchCharacter::SetAIPerceptionStimuliSourceEnabled(bool bIsEnabled)
{
	if (!PerceptionStimuliSourceComponent) return;
	if (bIsEnabled)
	{
		PerceptionStimuliSourceComponent->RegisterWithPerceptionSystem();
	}
	else
	{
		PerceptionStimuliSourceComponent->UnregisterFromPerceptionSystem();
	}
}




// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GenericTeamAgentInterface.h"
#include "CrunchCharacter.generated.h"

class UAIPerceptionStimuliSourceComponent;
struct FGameplayTag;
class UWidgetComponent;
class UAttributeSet;

UCLASS()
class CRUNCH_API ACrunchCharacter : public ACharacter, public IAbilitySystemInterface, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACrunchCharacter();
	void InitAbilityActorInfoOnServer();
	void InitAbilityActorInfoOnClient();

	virtual void PossessedBy(AController* NewController) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	bool IsLocallyControlledByPlayer() const;
private:

	/*******************************************************/
	/*					Gameplay Ability				   */
	/*******************************************************/
public:
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override {return CrunchAbilitySystemComponent;}
	
private:
	void BindAbilitySystemDelegate();
	void DeathTagUpdated(const FGameplayTag Tag, int32 NewCount);
	UPROPERTY(VisibleAnywhere, Category = "Ability System")
	TObjectPtr<UAbilitySystemComponent> CrunchAbilitySystemComponent;
	
	UPROPERTY(VisibleAnywhere, Category = "Ability System")
	TObjectPtr<UAttributeSet> CrunchAttributeSet;

	/*******************************************************/
	/*					UI								   */
	/*******************************************************/
public:
	void ConfigOverheadAttrBar();
private:
	UPROPERTY(VisibleDefaultsOnly, Category = "UI")
	TObjectPtr<UWidgetComponent> OverheadAttrBar;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	float OverheadVisibilityPeriod = 1.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	float OverheadVisibilityRangeSquared = 1000000.f;
	
	FTimerHandle OverheadAttrBarTimerHandle;

	void UpdateOverheadVisibily();

	/*******************************************************/
	/*					Death							   */
	/*******************************************************/
private:
	UPROPERTY(EditDefaultsOnly, Category = "Death Montage")
	TObjectPtr<UAnimMontage> DeathMontage;
	void PlayDeathMontage();
	void SetStatusGaugeEnabled(bool bEnabled);

	FTransform MeshInitRelativeTransform;
	
	FTimerHandle DeathTimerHandle;
	void DeathMontageFinished();
	void SetRagdollEnabled(bool bEnabled);
	
	void StartDeath();
	void Respawn();
	virtual void OnDead();
	virtual void OnRespawn();

	/*******************************************************/
	/*					team							   */
	/*******************************************************/
public:
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;

private:
	UPROPERTY(Replicated, VisibleAnywhere, Category = "Team")
	FGenericTeamId TeamID;

	/*******************************************************/
	/*					AI							   */
	/*******************************************************/

private:
	void SetAIPerceptionStimuliSourceEnabled(bool bIsEnabled);
	UPROPERTY()
	TObjectPtr<UAIPerceptionStimuliSourceComponent> PerceptionStimuliSourceComponent;

};

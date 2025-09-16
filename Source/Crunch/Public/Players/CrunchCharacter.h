// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "CrunchCharacter.generated.h"

class UWidgetComponent;
class UAttributeSet;

UCLASS()
class CRUNCH_API ACrunchCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACrunchCharacter();
	void InitAbilityActorInfoOnServer();
	void InitAbilityActorInfoOnClient();

	virtual void PossessedBy(AController* NewController) override;
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
};

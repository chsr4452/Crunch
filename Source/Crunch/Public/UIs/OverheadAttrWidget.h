// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OverheadAttrWidget.generated.h"

class UAbilitySystemComponent;
class UAttributeWidget;
/**
 * 
 */
UCLASS()
class CRUNCH_API UOverheadAttrWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void ConfigASC(UAbilitySystemComponent* ASC);
private:
	UPROPERTY(meta = (BindWidget))
    TObjectPtr<UAttributeWidget> HealthBar;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UAttributeWidget> ManaBar;
};

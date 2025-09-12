// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AttributeWidget.generated.h"

struct FOnAttributeChangeData;
struct FGameplayAttribute;
class UAbilitySystemComponent;
class UTextBlock;
class UProgressBar;
/**
 * 
 */
UCLASS()
class CRUNCH_API UAttributeWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	void BindToAttribute(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayAttribute& InAttribute, const FGameplayAttribute& InMaxAttribute);
	void SetProgressBarValue(float NewValue, float NewMaxValue);
private:
	void AttrChanged(const FOnAttributeChangeData& ChangedData);
	void MaxAttrChanged(const FOnAttributeChangeData& ChangedData);

	float CachedValue;
	float CachedMaxValue;
	
	UPROPERTY(EditAnywhere, Category = "Visual")
	FLinearColor ProgressBarColor;
	
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	TObjectPtr<UProgressBar> ProgressBar;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	TObjectPtr<UTextBlock> ValueBox;
};

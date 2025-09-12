// Fill out your copyright notice in the Description page of Project Settings.


#include "UIs/AttributeWidget.h"

#include "AbilitySystemComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UAttributeWidget::NativeConstruct()
{
	Super::NativeConstruct();
	ProgressBar->SetFillColorAndOpacity(ProgressBarColor);
}

void UAttributeWidget::BindToAttribute(UAbilitySystemComponent* AbilitySystemComponent,
	const FGameplayAttribute& InAttribute, const FGameplayAttribute& InMaxAttribute)
{
	if (AbilitySystemComponent)
	{
		bool bValueFound;
		bool bMaxValueFound;
		float Value = AbilitySystemComponent->GetGameplayAttributeValue(InAttribute, bValueFound);
		float MaxValue = AbilitySystemComponent->GetGameplayAttributeValue(InMaxAttribute, bMaxValueFound);
		if (bValueFound && bMaxValueFound)
		{
			SetProgressBarValue(Value, MaxValue);
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(InAttribute).AddUObject(this, &UAttributeWidget::AttrChanged);
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(InMaxAttribute).AddUObject(this, &UAttributeWidget::MaxAttrChanged);
		}
	}
}

void UAttributeWidget::SetProgressBarValue(float NewValue, float NewMaxValue)
{
	CachedValue = NewValue;
	CachedMaxValue = NewMaxValue;
	if (NewMaxValue == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s:, NewMaxValue is 0"), *GetName());
		return;
	}
	float NewPercent = NewValue / NewMaxValue;
	ProgressBar->SetPercent(NewPercent);

	FNumberFormattingOptions Options = FNumberFormattingOptions().SetMaximumFractionalDigits(0);

	ValueBox->SetText(
		FText::Format(
			FTextFormat::FromString(TEXT("{0} / {1}")),
			FText::AsNumber(NewValue, &Options),
			FText::AsNumber(NewMaxValue, &Options)));
}

void UAttributeWidget::AttrChanged(const FOnAttributeChangeData& ChangedData)
{
	SetProgressBarValue(ChangedData.NewValue, CachedMaxValue);
}

void UAttributeWidget::MaxAttrChanged(const FOnAttributeChangeData& ChangedData)
{
	SetProgressBarValue(CachedValue, ChangedData.NewValue);
}

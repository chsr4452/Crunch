// Fill out your copyright notice in the Description page of Project Settings.


#include "UIs/AttributeWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UAttributeWidget::NativeConstruct()
{
	Super::NativeConstruct();
	ProgressBar->SetFillColorAndOpacity(ProgressBarColor);
}

void UAttributeWidget::SetProgressBarValue(float NewValue, float NewMaxValue)
{
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

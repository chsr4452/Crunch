// Fill out your copyright notice in the Description page of Project Settings.


#include "UIs/OverheadAttrWidget.h"
#include "GameplayAbilitySystems/CrunchAttributeSet.h"
#include "UIs/AttributeWidget.h"

void UOverheadAttrWidget::ConfigASC(UAbilitySystemComponent* ASC)
{
	if (ASC)
	{
		HealthBar->BindToAttribute(ASC, UCrunchAttributeSet::GetHealthAttribute(), UCrunchAttributeSet::GetMaxHealthAttribute());
		ManaBar->BindToAttribute(ASC, UCrunchAttributeSet::GetManaAttribute(), UCrunchAttributeSet::GetMaxManaAttribute());
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "UIs/GameplayWidget.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "UIs/AttributeWidget.h"
#include "GameplayAbilitySystems/CrunchAttributeSet.h"

void UGameplayWidget::NativeConstruct()
{
	Super::NativeConstruct();
	UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwningPlayerPawn());
	if (AbilitySystemComponent)
	{
		HealthBar->BindToAttribute(AbilitySystemComponent, UCrunchAttributeSet::GetHealthAttribute(), UCrunchAttributeSet::GetMaxHealthAttribute());
		ManaBar->BindToAttribute(AbilitySystemComponent, UCrunchAttributeSet::GetManaAttribute(), UCrunchAttributeSet::GetMaxManaAttribute());
	}
}

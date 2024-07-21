// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystemComponent.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	UAuraAttributeSet* Attr = CastChecked<UAuraAttributeSet>(AttributeSet);
	OnHealthChanged.Broadcast(Attr->GetHealth());
	OnMaxHealthChanged.Broadcast(Attr->GetMaxHealth());

	OnManaChanged.Broadcast(Attr->GetMana());
	OnMaxManaChanged.Broadcast(Attr->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	UAuraAttributeSet* Attr = CastChecked<UAuraAttributeSet>(AttributeSet);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attr->GetHealthAttribute())
		.AddLambda([this](const FOnAttributeChangeData& Data) {
				OnHealthChanged.Broadcast(Data.NewValue);
			});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attr->GetMaxHealthAttribute())
		.AddLambda([this](const FOnAttributeChangeData& Data) {
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attr->GetManaAttribute())
		.AddLambda([this](const FOnAttributeChangeData& Data) {
				OnManaChanged.Broadcast(Data.NewValue);
			});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attr->GetMaxManaAttribute())
		.AddLambda([this](const FOnAttributeChangeData& Data) {
				OnMaxManaChanged.Broadcast(Data.NewValue);
			});

	check(MessageWidgetDataTable)

	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda(
		[this](const FGameplayTagContainer& AssetTags) {
			
			for (const FGameplayTag& Tag : AssetTags)
			{

				//if Tags are under Message Tag, find DataTable Row And broadCast;
				if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag("Message")))
				{
					FUIWidgetRow* row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);

					MessageWidgetRowDelegate.Broadcast(*row);
				}

				//FTEXT("")
			}
		}
	);
}
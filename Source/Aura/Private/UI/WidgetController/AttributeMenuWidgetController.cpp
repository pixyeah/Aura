// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "AuraGameplayTags.h"


void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);

	for (auto& Pair : AS->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value())
			.AddLambda(
				[this,Pair,AS](const FOnAttributeChangeData& Data) {
					FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoByTag(Pair.Key,true);
					Info.AttributeValue = Pair.Value().GetNumericValue(AS);
					AttributeDelegate.Broadcast(Info);
				}
			);
	}

}


void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	UAuraAttributeSet* AuraAttrSet = CastChecked<UAuraAttributeSet>(AttributeSet);

	check(AuraAttrSet);
	check(AttributeInfo);

	//method 3
	for (auto& Pair : AuraAttrSet->TagsToAttributes)
	{
		FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoByTag(Pair.Key,true);
		//Info.AttributeValue = Pair.Value.Execute().GetNumericValue(AuraAttrSet);
		Info.AttributeValue = Pair.Value().GetNumericValue(AuraAttrSet);
		AttributeDelegate.Broadcast(Info);
	}

	//method 2
	//for (FAuraAttributeInfo& AttrInfo : AttributeInfo->AttributeInfomation)
	//{
	//	AttrInfo.AttributeValue = AuraAttrSet->TagsToAttributes[AttrInfo.AttributeTag]
	//		.Execute()
	//		.GetNumericValue(AuraAttrSet);
	//	AttributeDelegate.Broadcast(AttrInfo);
	//}


	//method 1
	//FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoByTag(FAuraGameplayTags::Get().Attribute_Primary_Strength, true);
	//Info.AttributeValue = AuraAttrSet->GetStrength();

}

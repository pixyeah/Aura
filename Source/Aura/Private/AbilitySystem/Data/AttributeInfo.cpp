// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/AttributeInfo.h"

FAuraAttributeInfo UAttributeInfo::FindAttributeInfoByTag(const FGameplayTag& AttributeTag, bool bLogNotFound = true) const
{
	for (const FAuraAttributeInfo& Info : AttributeInfomation)
	{
		if (AttributeTag.MatchesTagExact(Info.AttributeTag))
		{
			return Info;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find Info by AttributeTag: %s on AttributeInfo: %s"),*AttributeTag.ToString(), *GetNameSafe(this));
	}

	return FAuraAttributeInfo();
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraGameplayTags.h"
#include "GameplayTagsManager.h"

FAuraGameplayTags FAuraGameplayTags::GameplayTags;

void FAuraGameplayTags::InitalizeNativeGameplayTags()
{
	GameplayTags.Attribute_Primary_Strength = UGameplayTagsManager::Get()
		.AddNativeGameplayTag(FName("Attributes.Primary.Strength"), 
			FString(TEXT("")));
	GameplayTags.Attribute_Primary_Intelligence = UGameplayTagsManager::Get()
		.AddNativeGameplayTag(FName("Attributes.Primary.Intelligence"), 
			FString(TEXT("")));
	GameplayTags.Attribute_Primary_Resilience = UGameplayTagsManager::Get()
		.AddNativeGameplayTag(FName("Attributes.Primary.Resilience"), 
			FString(TEXT("")));
	GameplayTags.Attribute_Primary_Vigor = UGameplayTagsManager::Get()
		.AddNativeGameplayTag(FName("Attributes.Primary.Vigor"), 
			FString(TEXT("")));



	GameplayTags.Attribute_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.Armor"),FString(TEXT("")));

	GameplayTags.Attribute_Secondary_ArmorPenetration = UGameplayTagsManager::Get().
		AddNativeGameplayTag(FName("Attributes.Secondary.ArmorPenetration"), 
			FString(TEXT("")));
	GameplayTags.Attribute_Secondary_BlockChance = UGameplayTagsManager::Get().
		AddNativeGameplayTag(FName("Attributes.Secondary.BlockChance"), 
			FString(TEXT("")));
	GameplayTags.Attribute_Secondary_CriticalHitChance = UGameplayTagsManager::Get().
		AddNativeGameplayTag(FName("Attributes.Secondary.CriticalHitChance"), 
			FString(TEXT("")));
	GameplayTags.Attribute_Secondary_CriticalHitDamage = UGameplayTagsManager::Get().
		AddNativeGameplayTag(FName("Attributes.Secondary.CriticalHitDamage"), 
			FString(TEXT("")));
	GameplayTags.Attribute_Secondary_CriticalHitResistance = UGameplayTagsManager::Get().
		AddNativeGameplayTag(FName("Attributes.Secondary.CriticalHitResistance"), 
			FString(TEXT("")));
	GameplayTags.Attribute_Secondary_HealthRegeneration = UGameplayTagsManager::Get()
		.AddNativeGameplayTag(FName("Attributes.Secondary.HealthRegeneration"), 
			FString(TEXT("")));
	GameplayTags.Attribute_Secondary_ManaRegeneration = UGameplayTagsManager::Get()
		.AddNativeGameplayTag(FName("Attributes.Secondary.ManaRegeneration"), 
			FString(TEXT("")));

	GameplayTags.Attribute_Secondary_MaxHealth = UGameplayTagsManager::Get()
		.AddNativeGameplayTag(FName("Attributes.Secondary.MaxHealth"),
		FString(TEXT("player can taken damageAmount until dead.")));

	GameplayTags.Attribute_Secondary_MaxMana = UGameplayTagsManager::Get()
		.AddNativeGameplayTag(FName("Attributes.Secondary.MaxMana"),
		FString(TEXT("player can spell . resources")));
}

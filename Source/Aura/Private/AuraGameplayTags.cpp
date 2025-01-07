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

	//Input Tag

	GameplayTags.InputTag_LMB = UGameplayTagsManager::Get()
		.AddNativeGameplayTag(FName("InputTag.LMB"),
			FString(TEXT("Input tag for left mouse button.")));
	GameplayTags.InputTag_RMB = UGameplayTagsManager::Get()
		.AddNativeGameplayTag(FName("InputTag.RMB"),
			FString(TEXT("Input tag for right mouse button.")));
	GameplayTags.InputTag_1 = UGameplayTagsManager::Get()
		.AddNativeGameplayTag(FName("InputTag.1"),
			FString(TEXT("Input tag for one key.")));
	GameplayTags.InputTag_2 = UGameplayTagsManager::Get()
		.AddNativeGameplayTag(FName("InputTag.2"),
			FString(TEXT("Input tag for two key.")));
	GameplayTags.InputTag_3 = UGameplayTagsManager::Get()
		.AddNativeGameplayTag(FName("InputTag.3"),
			FString(TEXT("Input tag for three key.")));
	GameplayTags.InputTag_4 = UGameplayTagsManager::Get()
		.AddNativeGameplayTag(FName("InputTag.4"),
			FString(TEXT("Input tag for four key.")));


	GameplayTags.Damage = UGameplayTagsManager::Get()
		.AddNativeGameplayTag(FName("Damage"),
			FString(TEXT("Damage.")));

	/*
	 *
	 * Damage Types
	 */

	GameplayTags.Damage_Fire = UGameplayTagsManager::Get()
		.AddNativeGameplayTag(FName("Damage.Fire"),
			FString(TEXT("fire damage type")));
	GameplayTags.Damage_Lightning = UGameplayTagsManager::Get()
		.AddNativeGameplayTag(FName("Damage.Lightning"),
			FString(TEXT("Lightning damage type")));
	GameplayTags.Damage_Arcane = UGameplayTagsManager::Get()
		.AddNativeGameplayTag(FName("Damage.Arcane"),
			FString(TEXT("Arcane damage type")));
	GameplayTags.Damage_Physical = UGameplayTagsManager::Get()
		.AddNativeGameplayTag(FName("Damage.Physical"),
			FString(TEXT("Physical damage type")));

	/*
	 *
	 * Damage Resistance
	 */
	GameplayTags.Attribute_Resistance_Fire = UGameplayTagsManager::Get()
		.AddNativeGameplayTag(FName("Attributes.Resistance.Fire"),
			FString(TEXT("Resistance to fire Damage.")));
		GameplayTags.Attribute_Resistance_Lightning = UGameplayTagsManager::Get()
		.AddNativeGameplayTag(FName("Attributes.Resistance.Lightning"),
			FString(TEXT("Resistance to Lightning Damage.")));
	GameplayTags.Attribute_Resistance_Arcane = UGameplayTagsManager::Get()
		.AddNativeGameplayTag(FName("Attributes.Resistance.Arcane"),
			FString(TEXT("Resistance to Arcane Damage.")));
	GameplayTags.Attribute_Resistance_Physical = UGameplayTagsManager::Get()
		.AddNativeGameplayTag(FName("Attributes.Resistance.Physical"),
			FString(TEXT("Resistance to Physical Damage.")));

	GameplayTags.DamageTypes.Add(GameplayTags.Damage_Fire,GameplayTags.Attribute_Resistance_Fire);
	GameplayTags.DamageTypes.Add(GameplayTags.Damage_Lightning, GameplayTags.Attribute_Resistance_Lightning);
	GameplayTags.DamageTypes.Add(GameplayTags.Damage_Arcane, GameplayTags.Attribute_Resistance_Arcane);
	GameplayTags.DamageTypes.Add(GameplayTags.Damage_Physical, GameplayTags.Attribute_Resistance_Physical);

	GameplayTags.Effects_HitReact = UGameplayTagsManager::Get()
		.AddNativeGameplayTag(FName("Effects.HitReact"),
			FString(TEXT("enemy hit react.")));

	GameplayTags.Abilities_Attack = UGameplayTagsManager::Get()
		.AddNativeGameplayTag(FName("Abilities.Attack"),
			FString(TEXT("Abilities to attack.")));
}

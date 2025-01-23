// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

void UAuraDamageGameplayAbility::CauseDamage(AActor* TargetActor)
{
	FGameplayEffectSpecHandle DamageSpecHandle =  MakeOutgoingGameplayEffectSpec(DamageEffectClass);
	for (TTuple<FGameplayTag, FScalableFloat> Pairs : DamageType)
	{
		const float ScaledDamage = Pairs.Value.GetValueAtLevel(GetAbilityLevel());
		UE_LOG(LogTemp, Display, TEXT("%s is cause damage %f"),*GetAvatarActorFromActorInfo()->GetActorNameOrLabel(),ScaledDamage);
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, Pairs.Key, ScaledDamage);
	}

	UAbilitySystemComponent* TargetASC =  UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(), TargetASC);
}

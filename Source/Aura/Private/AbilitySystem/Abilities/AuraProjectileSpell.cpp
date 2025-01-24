// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AuraProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "Actor/AuraProjectile.h"
#include "Interact/CombatInterface.h"
#include "Kismet/KismetSystemLibrary.h"

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	//UKismetSystemLibrary::PrintString(this, FString("Activate Ability fromC++"),
		//true, true, FLinearColor::Green, 6);

	bIsServer = HasAuthority(&ActivationInfo);
}

void UAuraProjectileSpell::SpawnProjectile(const FVector& TargetLocation)
{
	if (!GetAvatarActorFromActorInfo()->HasAuthority()) return;
	if (ICombatInterface* CombatIf = Cast<ICombatInterface>(GetAvatarActorFromActorInfo()))
	{
		AActor* OwningActor = GetOwningActorFromActorInfo();
		FTransform SpawnTransform;

		const FVector WeaponLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(),
			FAuraGameplayTags::Get().Montage_Attack_Weapon);
		FRotator Rotation = (TargetLocation - WeaponLocation).Rotation();
		//Rotation.Pitch = 0;
		SpawnTransform.SetLocation(WeaponLocation);
		SpawnTransform.SetRotation(Rotation.Quaternion());

		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
			ProjectileClass,
			SpawnTransform,
			OwningActor,
			Cast<APawn>(OwningActor),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
		FGameplayEffectContextHandle EffectContextHandle = SourceASC->MakeEffectContext();
		EffectContextHandle.SetAbility(this);
		EffectContextHandle.AddSourceObject(Projectile);
		TArray<TWeakObjectPtr<AActor>> Actors;
		Actors.Add(Projectile);
		EffectContextHandle.AddActors(Actors);
		FHitResult HitResult;
		EffectContextHandle.AddHitResult(HitResult);
		FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), EffectContextHandle);

		FAuraGameplayTags AuraGameplayTags = FAuraGameplayTags::Get();
		for (TTuple<FGameplayTag,FScalableFloat>& Pair : DamageType)
		{
			const float ScalarDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel());
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Pair.Key, ScalarDamage);
			
		}

		Projectile->DamageEffectSpecHandle = SpecHandle;
		Projectile->FinishSpawning(SpawnTransform);
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AuraProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Actor/AuraProjectile.h"
#include "Interact/CombatInterface.h"
#include "Kismet/KismetSystemLibrary.h"

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UKismetSystemLibrary::PrintString(this, FString("Activate Ability fromC++"),
		true, true, FLinearColor::Green, 6);


	bIsServer = HasAuthority(&ActivationInfo);

}

void UAuraProjectileSpell::SpawnProjectile(const FVector& TargetLocation)
{
	if (!bIsServer) return;
	if (ICombatInterface* CombatIf = Cast<ICombatInterface>(GetAvatarActorFromActorInfo()))
	{
		AActor* OwningActor = GetOwningActorFromActorInfo();
		FTransform SpawnTransform;

		const FVector WeaponLocation = CombatIf->GetCombatSocketLocation();
		FRotator Rotation = (TargetLocation - WeaponLocation).Rotation();
		Rotation.Pitch = 0;
		SpawnTransform.SetLocation(WeaponLocation);
		SpawnTransform.SetRotation(Rotation.Quaternion());

		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
			ProjectileClass,
			SpawnTransform,
			OwningActor,
			Cast<APawn>(OwningActor),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
		FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), SourceASC->MakeEffectContext());
		Projectile->DamageEffectSpecHandle = SpecHandle;
		Projectile->FinishSpawning(SpawnTransform);
	}
}

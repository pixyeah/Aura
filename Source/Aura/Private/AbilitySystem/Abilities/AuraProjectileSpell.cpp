// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AuraProjectileSpell.h"

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

void UAuraProjectileSpell::SpawnProjectile()
{
	if (!bIsServer) return;
	if (ICombatInterface* CombatIf = Cast<ICombatInterface>(GetAvatarActorFromActorInfo()))
	{
		AActor* OwningActor = GetOwningActorFromActorInfo();
		FTransform SpawnTransform;

		//TODO: Set the projectile rotation.

		const FVector WeaponLocation = CombatIf->GetCombatSocketLocation();
		SpawnTransform.SetLocation(WeaponLocation);
		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
			ProjectileClass,
			SpawnTransform,
			OwningActor,
			Cast<APawn>(OwningActor),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		//TODO: Give the projectile a gameplay effect spec for causing damage.

		Projectile->FinishSpawning(SpawnTransform);
	}
}

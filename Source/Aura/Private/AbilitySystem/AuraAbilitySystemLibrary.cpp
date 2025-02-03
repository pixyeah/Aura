// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilityTypes.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Game/AuraGameModeBase.h"
#include "Player/AuraPlayerState.h"
#include "Engine/OverlapResult.h"
#include <Interact/CombatInterface.h>

UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldObject)
{

    if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldObject, 0))
    {

        if (AAuraHUD* AuraHUD = PlayerController->GetHUD<AAuraHUD>())
        {
            AAuraPlayerState* PS = PlayerController->GetPlayerState<AAuraPlayerState>();
            UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
            UAttributeSet* AttributeSet = PS->GetAttributeSet();
            const FWidgetControllerParam WidgetControllerParam(PlayerController, PS, ASC, AttributeSet);
            return AuraHUD->GetOverlayWidgetController(WidgetControllerParam);
        }
    }

    return nullptr;
}

UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuController(const UObject* WorldObject)
{
    if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldObject, 0))
    {

        if (AAuraHUD* AuraHUD = PlayerController->GetHUD<AAuraHUD>())
        {
            AAuraPlayerState* PS = PlayerController->GetPlayerState<AAuraPlayerState>();
            UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
            UAttributeSet* AttributeSet = PS->GetAttributeSet();
            const FWidgetControllerParam WidgetControllerParam(PlayerController, PS, ASC, AttributeSet);
            return AuraHUD->GetAttributeMenuWidgetController(WidgetControllerParam);
        }
    }
    return nullptr;
}

void UAuraAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldObject, ECharacterClass CharacterClass, float Level,UAbilitySystemComponent* ASC)
{
    UCharacterClassInfo* ClassInfo = GetCharacterClassInfo(WorldObject);
    if (!ClassInfo) return;
    FCharacterClassDefaultInfo ClassDefaultInfo = ClassInfo->GetClassDefaultInfo(CharacterClass);

    AActor* AvatarActor = ASC->GetAvatarActor();

    FGameplayEffectContextHandle PrimaryEffectHandle = ASC->MakeEffectContext();
    PrimaryEffectHandle.AddSourceObject(AvatarActor);
    FGameplayEffectSpecHandle PrimaryAttributesSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level, PrimaryEffectHandle);
    ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data.Get());


    FGameplayEffectContextHandle SecondaryEffectHandle = ASC->MakeEffectContext();
    SecondaryEffectHandle.AddSourceObject(AvatarActor);
    FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = ASC->MakeOutgoingSpec(ClassInfo->SecondaryAttributes, Level, SecondaryEffectHandle);
    ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());

    FGameplayEffectContextHandle VitalEffectHandle = ASC->MakeEffectContext();
    VitalEffectHandle.AddSourceObject(AvatarActor);
    FGameplayEffectSpecHandle VitalAttributesSpecHandle = ASC->MakeOutgoingSpec(ClassInfo->VitalAttributes, Level, VitalEffectHandle);
    ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get());
}

void UAuraAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass)
{
    UCharacterClassInfo* ClassInfo = GetCharacterClassInfo(WorldObject);
    if (ClassInfo == nullptr) return;

    for (TSubclassOf<UGameplayAbility> Ability : ClassInfo->CommonAbilities)
    {
        FGameplayAbilitySpec Spec = FGameplayAbilitySpec(Ability, 1);
        ASC->GiveAbility(Spec);
    }

    const FCharacterClassDefaultInfo& DefaultInfo = ClassInfo->GetClassDefaultInfo(CharacterClass);
    for (auto AbilityClass : DefaultInfo.StartupAbilities)
    {
        if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(ASC->GetAvatarActor()))
        {
            FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, CombatInterface->GetPlayerLevel());
            ASC->GiveAbility(AbilitySpec);
        }
    }

}

UCharacterClassInfo* UAuraAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldObject)
{

    AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldObject));
    if (!AuraGameMode) return nullptr;

    UCharacterClassInfo* ClassInfo = AuraGameMode->CharacterClassInfo;
    return ClassInfo;
}

bool UAuraAbilitySystemLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
    if (const FAuraGameplayEffectContext* AuraEffectHandle = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
    {
        return AuraEffectHandle->IsBlockedHit();
    }
    return false;
}

bool UAuraAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
    if (const FAuraGameplayEffectContext* AuraEffectHandle = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
    {
        return AuraEffectHandle->IsCriticalHit();
    }
    return false;
}

void UAuraAbilitySystemLibrary::GetLivePlayersWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin)
{
    FCollisionQueryParams SphereParams;
    SphereParams.AddIgnoredActors(ActorsToIgnore);

    TArray<FOverlapResult> Overlaps;
    if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
    {
        World->OverlapMultiByObjectType(Overlaps,SphereOrigin,FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects),
            FCollisionShape::MakeSphere(Radius), SphereParams);

        for (FOverlapResult& Result : Overlaps)
        {
            AActor* ActorR = Result.GetActor();
            if (ActorR->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(ActorR))
            {
                OutOverlappingActors.AddUnique(ActorR);
            }
        }
    }

}

bool UAuraAbilitySystemLibrary::IsNotFriend(AActor* FirstActor, AActor* SecondActor)
{
    const bool FirstIsplayer = FirstActor->ActorHasTag(FName("Player"));
    const bool SecondIsplayer = SecondActor->ActorHasTag(FName("Player"));

    const bool IsBothPlayer = FirstIsplayer && SecondIsplayer;

    const bool IsBothEnemy = FirstActor->ActorHasTag(FName("Enemy")) && SecondActor->ActorHasTag(FName("Enemy"));

    return !IsBothPlayer || !IsBothEnemy;
}

void UAuraAbilitySystemLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle,
	bool bInIsCriticalHit)
{
    FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get());
    AuraEffectContext->SetIsCriticalHit(bInIsCriticalHit);

}

void UAuraAbilitySystemLibrary::SetIsBlockedHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit)
{
    FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get());
    AuraEffectContext->SetIsBlockedHit(bInIsBlockedHit);
}

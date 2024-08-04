// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AbilitySystemComponent.h"
#include "Player/AuraPlayerState.h"

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

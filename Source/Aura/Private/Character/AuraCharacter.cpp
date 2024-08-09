// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/AuraPlayerState.h"
#include "AbilitySystemComponent.h"
#include "UI/HUD/AuraHUD.h"
#include "Player/AuraPlayerController.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"

AAuraCharacter::AAuraCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 400.0f, 0.0f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//UE_LOG(LogTemp, Warning, TEXT("PossessedBy"));
	//Init Actor info for server
	InitAbilityActorInfo();
	AddCharacterAbilities();
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	//Init Actor info for Client
	InitAbilityActorInfo();
	//UE_LOG(LogTemp, Warning, TEXT("OnRep_PlayerState"));
}

int32 AAuraCharacter::GetPlayerLevel()
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);

	return AuraPlayerState->GetPlayerLevel();
}

void AAuraCharacter::InitAbilityActorInfo()
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	UAbilitySystemComponent* AbilitySystem = AuraPlayerState->GetAbilitySystemComponent();
	check(AbilitySystem);
	AbilitySystem->InitAbilityActorInfo(AuraPlayerState, this);
	Cast<UAuraAbilitySystemComponent>(AbilitySystem)->AbilityActorInfoSet();
	AbilitySystemComponent = AbilitySystem;
	AttributeSet = AuraPlayerState->GetAttributeSet();

	//init aurahud
	if (AAuraPlayerController* AuraController = Cast<AAuraPlayerController>(GetController()))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(AuraController->GetHUD()))
		{
			AuraHUD->InitOverlay(Cast<APlayerController>(GetController()),
				AuraPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}

	//init attributes
	InitDefaultAttribute();
}

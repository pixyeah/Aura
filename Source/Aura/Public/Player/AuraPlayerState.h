// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "AuraPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	AAuraPlayerState();

	UPROPERTY(VisibleAnyWhere)
	TObjectPtr<class UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<class UAttributeSet> AttributeSet;

	// 通过 IAbilitySystemInterface 继承
	UAbilitySystemComponent* GetAbilitySystemComponent() const override;



	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	int32 GetPlayerLevel();

private :

	UPROPERTY(EditAnywhere, ReplicatedUsing = OnRep_Level)
	int32 Level = 1;

	UFUNCTION()
	void OnRep_Level(int32 OldLevel);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AttributeMenuWidgetController.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;
class APlayerController;
class APlayerState;
struct FAuraAttributeInfo;
class UAttributeInfo;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoSignature, const FAuraAttributeInfo&, Info);

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class AURA_API UAttributeMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:


	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable,Category="GAS|Attributes")
	FAttributeInfoSignature AttributeDelegate;
	
protected:
	virtual void BroadcastInitialValues() override;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAttributeInfo> AttributeInfo;

};

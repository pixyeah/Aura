// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AuraWidgetController.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;

USTRUCT(BlueprintType)
struct FWidgetControllerParam
{
	GENERATED_BODY()

	FWidgetControllerParam() {}

	FWidgetControllerParam(APlayerController* Controller, APlayerState* State,
		UAbilitySystemComponent* AbilitySystemComp, UAttributeSet* AttrSet)
		:PlayerController(Controller), PlayerState(State),
		AbilitySystemComponent(AbilitySystemComp), AttributeSet(AttrSet)
	{
	}
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<APlayerState> PlayerState;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr< UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr< UAttributeSet> AttributeSet;
};

/**
 * 
 */
UCLASS()
class AURA_API UAuraWidgetController : public UObject
{
	GENERATED_BODY()

public :
	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParams(const FWidgetControllerParam& Params);

	virtual void BindCallbacksToDependencies();

protected:
	
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<APlayerState> PlayerState;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr< UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr< UAttributeSet> AttributeSet;

	virtual void BroadcastInitialValues();
};

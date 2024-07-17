// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AuraWidgetController.h"

void UAuraWidgetController::SetWidgetControllerParams(const FWidgetControllerParam& Params)
{
	PlayerState = Params.PlayerState;
	PlayerController = Params.PlayerController;
	AttributeSet = Params.AttributeSet;
	AbilitySystemComponent = Params.AbilitySystemComponent;

}

void UAuraWidgetController::BindCallbacksToDependencies()
{
}

void UAuraWidgetController::BroadcastInitialValues()
{
}

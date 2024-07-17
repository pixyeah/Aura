// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/AuraHUD.h"
#include "UI/Widget/AuraUserWidget.h"
#include "UI/WidgetController/OverlayWidgetController.h"

UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParam& Param)
{

	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(Param);
		OverlayWidgetController->BindCallbacksToDependencies();

	}
	return OverlayWidgetController;
}

void AAuraHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("HBP_AuraHUD. OverlayWidgetClass is uninitialized!"));
	checkf(OverlayWidgetControllerClass, TEXT("HBP_AuraHUD. OverlayWidgetControllerClass is uninitialized!"));

	UUserWidget* UserWidget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass, "MainOver");
	OverlayWidget = Cast<UAuraUserWidget>(UserWidget);

	const FWidgetControllerParam WidgetControllerParam(PC, PS, ASC, AS);
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParam);
	OverlayWidget->SetWidgetController(WidgetController);
	WidgetController->BroadcastInitialValues();

	OverlayWidget->AddToViewport();
}


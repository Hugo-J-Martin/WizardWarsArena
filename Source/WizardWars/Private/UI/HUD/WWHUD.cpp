// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/WWHUD.h"
#include "UI/Widget/WWUserWidget.h"
#include "Blueprint/UserWidget.h"
#include "UI/WidgetController/HUDWidgetController.h"

UHUDWidgetController* AWWHUD::GetHUDWidgetController(const FWidgetControllerParams& WCParams)
{
	if (HUDWidgetController == nullptr)
	{
		HUDWidgetController = NewObject<UHUDWidgetController>(this, HUDWidgetControllerClass);
		HUDWidgetController->SetWidgetControllerParams(WCParams);
		HUDWidgetController->BindCallbacksToDependencies();
		return HUDWidgetController;
	}
	return HUDWidgetController;
}

void AWWHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(HUDWidgetClass, TEXT("HUD Widget Class uninitialized, please fill out BP_WWHUD"));
	checkf(HUDWidgetControllerClass, TEXT("HUD Widget Controller Class uninitialized, please fill out BP_WWHUD"));
	
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
	HUDWidget = Cast<UWWUserWidget>(Widget);

	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	UHUDWidgetController* WidgetController = GetHUDWidgetController(WidgetControllerParams);
	
	HUDWidget->SetWidgetController(WidgetController);
	WidgetController->BroadcastInitialValues();
	
	Widget->AddToViewport();
}


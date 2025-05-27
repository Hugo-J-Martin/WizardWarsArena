// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/WWWidgetController.h"

void UWWWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
}

void UWWWidgetController::BroadcastInitialValues()
{
	
}

void UWWWidgetController::BindCallbacksToDependencies()
{
}

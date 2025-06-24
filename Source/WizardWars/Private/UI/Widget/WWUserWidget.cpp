// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/WWUserWidget.h"

void UWWUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}

void UWWUserWidget::SetWeaponWidgetController(UObject* Controller)
{
	WeaponWidgetController = Controller;
	UE_LOG(LogTemp, Warning, TEXT("Setting Widget controller bruhhh"));
	if (WeaponWidgetController)
	{
		UE_LOG(LogTemp, Log, TEXT("WeaponWidgetControllerSet: WeaponWidgetController is valid"));
		WeaponWidgetControllerSet();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("WeaponWidgetControllerSet: WeaponWidgetController is NULL"));
	}
}



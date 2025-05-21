// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/WWPickupWidgetBase.h"

#include "Components/TextBlock.h"

void UWWPickupWidgetBase::SetPickupName(const FText& NewName)
{
	if (PickupNameText)
	{
		PickupNameText->SetText(NewName);
	}
}

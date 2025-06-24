// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/HUDWeaponWidgetController.h"

void UHUDWeaponWidgetController::BroadcastInitialValues()
{
	//Super::BroadcastInitialValues();
	const UWWGunAttributeSet* GunAttributes = CastChecked<UWWGunAttributeSet>(AttributeSet);
	OnAmmoChanged.Broadcast(GunAttributes->GetAmmo());
	OnReserveAmmoChanged.Broadcast(GunAttributes->GetReserveAmmo());
}

void UHUDWeaponWidgetController::BindCallbacksToDependencies()
{
	const UWWGunAttributeSet* GunAttributes = CastChecked<UWWGunAttributeSet>(AttributeSet);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GunAttributes->GetAmmoAttribute()).AddUObject(this, &UHUDWeaponWidgetController::AmmoChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GunAttributes->GetReserveAmmoAttribute()).AddUObject(this, &UHUDWeaponWidgetController::ReserveAmmoChanged);
}

void UHUDWeaponWidgetController::AmmoChanged(const FOnAttributeChangeData& Data) const
{
	UE_LOG(LogTemp, Warning, TEXT("AmmoChanged: NewValue = %f"), Data.NewValue);
	OnAmmoChanged.Broadcast(Data.NewValue);
}

void UHUDWeaponWidgetController::ReserveAmmoChanged(const FOnAttributeChangeData& Data) const
{
	UE_LOG(LogTemp, Warning, TEXT("ReserveAmmoChanged: NewValue = %f"), Data.NewValue);
	OnReserveAmmoChanged.Broadcast(Data.NewValue);
}

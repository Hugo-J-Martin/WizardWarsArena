// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/HUDWidgetController.h"

#include "AbilitySystem/WWAttributeSet.h"
#include "AbilitySystem/WWGunAttributeSet.h"
#include "Player/WWPlayerController.h"

void UHUDWidgetController::BroadcastInitialValues()
{
	const UWWAttributeSet* WWAttributeSet = CastChecked<UWWAttributeSet>(AttributeSet);
	AWWPlayerController* WWPC = CastChecked<AWWPlayerController>(PlayerController);
	//const UWWGunAttributeSet* WWGunAttributeSet = CastChecked<UWWGunAttributeSet>(WeaponAttributeSet);

	OnHealthChanged.Broadcast(WWAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(WWAttributeSet->GetMaxHealth());
	OnArmorChanged.Broadcast(WWAttributeSet->GetArmor());
	OnMaxArmorChanged.Broadcast(WWAttributeSet->GetMaxArmor());
	//OnAmmoChanged.Broadcast(WWGunAttributeSet->GetAmmo());
	//OnReserveAmmoChanged.Broadcast(WWGunAttributeSet->GetReserveAmmo());
	
	

	
}



void UHUDWidgetController::BindCallbacksToDependencies()
{
	const UWWAttributeSet* WWAttributeSet = CastChecked<UWWAttributeSet>(AttributeSet);
	//const UWWGunAttributeSet* WWGunAttributeSet = CastChecked<UWWGunAttributeSet>(WeaponAttributeSet);
	//if (const UWWGunAttributeSet* WWGunAttributeSet = Cast<UWWGunAttributeSet>(WeaponAttributeSet))
	//{
	//	OnAmmoChanged.Broadcast(WWGunAttributeSet->GetAmmo());
	//	OnReserveAmmoChanged.Broadcast(WWGunAttributeSet->GetReserveAmmo());
	//}

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		WWAttributeSet->GetHealthAttribute()).AddUObject(this, &UHUDWidgetController::HealthChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		WWAttributeSet->GetMaxHealthAttribute()).AddUObject(this, &UHUDWidgetController::MaxHealthChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		WWAttributeSet->GetArmorAttribute()).AddUObject(this, &UHUDWidgetController::ArmorChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		WWAttributeSet->GetMaxArmorAttribute()).AddUObject(this, &UHUDWidgetController::MaxArmorChanged);

	//AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
	//	WWGunAttributeSet->GetAmmoAttribute()).AddUObject(this, &UHUDWidgetController::AmmoChanged);
	
	//AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
	//	WWGunAttributeSet->GetReserveAmmoAttribute()).AddUObject(this, &UHUDWidgetController::ReserveAmmoChanged);

	//const AWWPlayerController* WWPC = CastChecked<AWWPlayerController>(PlayerController);

	
}



void UHUDWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthChanged.Broadcast(Data.NewValue);
}

void UHUDWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}

void UHUDWidgetController::ArmorChanged(const FOnAttributeChangeData& Data) const
{
	OnArmorChanged.Broadcast(Data.NewValue);
}

void UHUDWidgetController::MaxArmorChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxArmorChanged.Broadcast(Data.NewValue);
}



/**
void UHUDWidgetController::AmmoChanged(const FOnAttributeChangeData& Data) const
{
	OnAmmoChanged.Broadcast(Data.NewValue);
}

void UHUDWidgetController::ReserveAmmoChanged(const FOnAttributeChangeData& Data) const
{
	OnReserveAmmoChanged.Broadcast(Data.NewValue);
}
**/



// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/HUDWidgetController.h"
#include "Weapon/WWGunBase.h"
#include "HUDWeaponWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAmmoChangedSignature, float, NewAmmo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnReserveAmmoChangedSignature, float, NewReserveAmmo);

/**
 * 
 */



UCLASS()
class WIZARDWARS_API UHUDWeaponWidgetController : public UHUDWidgetController
{
	GENERATED_BODY()

public:

	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAmmoChangedSignature OnAmmoChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnReserveAmmoChangedSignature OnReserveAmmoChanged;

	UPROPERTY(BlueprintReadOnly, Category="Weapon")
	UTexture2D* WeaponIcon;

	UPROPERTY(BlueprintReadOnly, Category="Weapon")
	FString WeaponName;

protected:

	void AmmoChanged(const FOnAttributeChangeData& Data) const;
	void ReserveAmmoChanged(const FOnAttributeChangeData& Data) const;
	
};

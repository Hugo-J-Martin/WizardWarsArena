// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/WWWidgetController.h"
#include "HUDWidgetController.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxHealthChangedSignature, float, NewMaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnArmorChangedSignature, float, NewArmor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxArmorChangedSignature, float, NewMaxArmor);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class WIZARDWARS_API UHUDWidgetController : public UWWWidgetController
{
	GENERATED_BODY()
public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnHealthChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnMaxHealthChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnArmorChangedSignature OnArmorChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnMaxArmorChangedSignature OnMaxArmorChanged;


protected:
	void HealthChanged(const FOnAttributeChangeData& Data) const;
	void MaxHealthChanged(const FOnAttributeChangeData& Data) const;
	void ArmorChanged(const FOnAttributeChangeData& Data) const;
	void MaxArmorChanged(const FOnAttributeChangeData& Data) const;

	
};

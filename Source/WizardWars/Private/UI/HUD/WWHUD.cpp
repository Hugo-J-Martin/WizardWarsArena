// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/WWHUD.h"
#include "UI/Widget/WWUserWidget.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerState.h"
#include "UI/WidgetController/HUDWeaponWidgetController.h"
#include "Weapon/WWGunBase.h"
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

UHUDWeaponWidgetController* AWWHUD::GetHUDWeaponWidgetController(const FWidgetControllerParams& WCParams)
{
	if (HUDWeaponWidgetController == nullptr)
	{
		HUDWeaponWidgetController = NewObject<UHUDWeaponWidgetController>(this, HUDWeaponWidgetControllerClass);
		HUDWeaponWidgetController->SetWidgetControllerParams(WCParams);
		HUDWeaponWidgetController->BindCallbacksToDependencies();
		return HUDWeaponWidgetController;
	}
	return HUDWeaponWidgetController;
}

void AWWHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(HUDWidgetClass, TEXT("HUD Widget Class uninitialized, please fill out BP_WWHUD"));
	checkf(HUDWidgetControllerClass, TEXT("HUD Widget Controller Class uninitialized, please fill out BP_WWHUD"));
	
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
	HUDWidget = Cast<UWWUserWidget>(Widget);
	//HUDWidget = CreateWidget<UWWUserWidget>(GetWorld(), HUDWidgetClass);

	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS, nullptr); 
	UHUDWidgetController* WidgetController = GetHUDWidgetController(WidgetControllerParams);
	
	HUDWidget->SetWidgetController(WidgetController);
	WidgetController->BroadcastInitialValues();
	
	Widget->AddToViewport();
	//HUDWidget->AddToViewport();
}

void AWWHUD::InitWeaponOverlay(UAbilitySystemComponent* WeaponASC, UWWGunAttributeSet* WeaponAS, UTexture2D* WeaponIcon, FString WeaponName)
{

	if (!IsValid(HUDWidgetClass) || !IsValid(HUDWidgetControllerClass))
	{
		UE_LOG(LogTemp, Error, TEXT("HUDWidgetClass or HUDWidgetControllerClass not set!"));
		return;
	}

	APlayerController* PC = GetOwningPlayerController();
	if (!PC || !PC->IsLocalController()) // Ensures we're only on local client or listen server's local player
	{
		UE_LOG(LogTemp, Warning, TEXT("InitWeaponOverlay called on non-local controller. Skipping."));
		return;
	}

	APlayerState* PS = PC->PlayerState;
	if (!PS || !IsValid(WeaponASC) || !IsValid(WeaponAS))
	{
		UE_LOG(LogTemp, Warning, TEXT("InitWeaponOverlay missing valid parameters. PS: %s, ASC: %s, AS: %s"),
			PS ? *PS->GetName() : TEXT("null"),
			WeaponASC ? *WeaponASC->GetName() : TEXT("null"),
			WeaponAS ? *WeaponAS->GetName() : TEXT("null"));
		return;
	}

	const FWidgetControllerParams WeaponWidgetControllerParams(PC, PS, WeaponASC, WeaponAS, nullptr);
	UHUDWeaponWidgetController* WeaponWidgetController = NewObject<UHUDWeaponWidgetController>(this, HUDWeaponWidgetControllerClass);
	checkf(WeaponWidgetController, TEXT("WeaponWidgetController not created"));

	UE_LOG(LogTemp, Warning, TEXT("Creating new WeaponWidgetController of class: %s"), *WeaponWidgetController->GetClass()->GetName());

	WeaponWidgetController->SetWidgetControllerParams(WeaponWidgetControllerParams);
	WeaponWidgetController->WeaponIcon = WeaponIcon;
	WeaponWidgetController->WeaponName = WeaponName;

	WeaponWidgetController->BindCallbacksToDependencies(); //Where the delegates are bound

	HUDWidget->SetWeaponWidgetController(WeaponWidgetController);

	WeaponWidgetController->BroadcastInitialValues(); //Triggers initial ammo values
}




// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/HUD.h"
#include "UI/WidgetController/HUDWeaponWidgetController.h"
#include "Weapon/WWGunBase.h"
#include "WWHUD.generated.h"


class UHUDWidgetController;
class UHUDWeaponWidgetController;
class UWWUserWidget;
struct FWidgetControllerParams;
/**
 * 
 */
UCLASS()
class WIZARDWARS_API AWWHUD : public AHUD
{
	GENERATED_BODY()
public:

	UPROPERTY()
	TObjectPtr<UWWUserWidget> HUDWidget;

	UHUDWidgetController* GetHUDWidgetController(const FWidgetControllerParams& WCParams);

	UHUDWeaponWidgetController* GetHUDWeaponWidgetController(const FWidgetControllerParams& WCParams);

	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

	void InitWeaponOverlay(UAbilitySystemComponent* WeaponASC, UWWGunAttributeSet* WeaponAS, UTexture2D* WeaponIcon, FString WeaponName);



protected:
	
private:

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> HUDWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> HUDWeaponWidgetClass;

	UPROPERTY()
	TObjectPtr<UHUDWidgetController> HUDWidgetController;
	

	UPROPERTY()
	TObjectPtr<UHUDWeaponWidgetController> HUDWeaponWidgetController;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UHUDWidgetController> HUDWidgetControllerClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UHUDWeaponWidgetController> HUDWeaponWidgetControllerClass;
	
};

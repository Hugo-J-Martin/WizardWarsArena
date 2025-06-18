// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/HUD.h"
#include "WWHUD.generated.h"


class UHUDWidgetController;
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

	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

	//void OnWeaponEquipped(AWWGunBase* NewWeapon);

protected:
	
private:

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> HUDWidgetClass;

	UPROPERTY()
	TObjectPtr<UHUDWidgetController> HUDWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UHUDWidgetController> HUDWidgetControllerClass;
	
};

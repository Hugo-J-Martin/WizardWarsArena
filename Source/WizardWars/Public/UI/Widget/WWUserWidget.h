// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WWUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class WIZARDWARS_API UWWUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);

	UFUNCTION(BlueprintCallable)
	void SetWeaponWidgetController(UObject* Controller);
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WeaponWidgetController;

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();

	UFUNCTION(BlueprintImplementableEvent)
	void WeaponWidgetControllerSet();
	
	
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "UObject/NoExportTypes.h"
#include "WWWidgetController.generated.h"


class UAttributeSet;
class UAbilitySystemComponent;

USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()

	FWidgetControllerParams() {}
	FWidgetControllerParams(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
	: PlayerController(PC), PlayerState(PS), AbilitySystemComponent(ASC), AttributeSet(AS) {}
	//ADD 
	//ADD WeaponAbilitySystemComponent(WeaponASC), WeaponAttributeSet(WeaponAS)

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerState> PlayerState = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAttributeSet> AttributeSet = nullptr;

	//FOR WHEN I ADD WEAPON
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//TObjectPtr<UAbilitySystemComponent> WeaponAbilitySystemComponent = nullptr;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//TObjectPtr<UAttributeSet> WeaponAttributeSet = nullptr;
};
/**
 * 
 */
UCLASS()
class WIZARDWARS_API UWWWidgetController : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParams(const FWidgetControllerParams& WCParams);

	virtual void BroadcastInitialValues();

	virtual void BindCallbacksToDependencies();
protected:

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<APlayerState> PlayerState;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAttributeSet> AttributeSet;

	//UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	//TObjectPtr<UAbilitySystemComponent> WeaponAbilitySystemComponent;

	//UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	//TObjectPtr<UAttributeSet> WeaponAttributeSet;
};

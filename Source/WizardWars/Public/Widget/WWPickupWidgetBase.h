// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WWPickupWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class WIZARDWARS_API UWWPickupWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	// Set pickup name from c++
	UFUNCTION(BlueprintCallable, Category = "Pickup")
	void SetPickupName(const FText& NewName);

protected:
	// Variable set from c++, bind in BP
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "Pickup")
	class UTextBlock* PickupNameText;
};

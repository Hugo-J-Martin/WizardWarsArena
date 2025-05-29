// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "InteractionTextComponent.generated.h"


UCLASS()
class WIZARDWARS_API UInteractionTextComponent : public UWidgetComponent
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, meta = (AutoCreateRefTerm = "ItemText"))
	void SetInteractionText(const FText& Item);


};

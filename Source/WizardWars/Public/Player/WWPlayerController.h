// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "WWPlayerController.generated.h"


class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

/**
 * 
 */
UCLASS()
class WIZARDWARS_API AWWPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AWWPlayerController();
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> WWContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> CrouchAction;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float LeanSpeed = 5.f;

	void Move(const FInputActionValue& InputActionValue);
	void Look(const FInputActionValue& InputActionValue);
	void JumpPressed(const FInputActionValue& InputActionValue);
	void JumpReleased(const FInputActionValue& InputActionValue);
	void CrouchPressed(const FInputActionValue& InputActionValue);
	void CrouchReleased(const FInputActionValue& InputActionValue);

	
	void MovementLean(float ScaleVal);
	void UpdateLean(float LeanRoll);

	UPROPERTY(EditAnywhere, Category = "UI|Weapon")
	TObjectPtr<UWidgetComponent> PickupWidget;
	
};

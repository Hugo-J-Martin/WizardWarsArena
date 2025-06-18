// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Character/WWCharacter.h"
#include "GameFramework/PlayerController.h"
#include "Widget/InteractionTextComponent.h"
#include "WWPlayerController.generated.h"


class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class IInteractableInterface;
class UWWPickupWidgetBase;
/**
 * 
 */
UCLASS()
class WIZARDWARS_API AWWPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AWWPlayerController();
	virtual void PlayerTick(float DeltaTime) override;

	UFUNCTION(Client, Reliable)
	void ShowInteractionText(const FText& Item, AWWGunBase* Weapon);

	void ShowPickupWidget(const FText& Name);
	void HidePickupWidget();

	TScriptInterface<IInteractableInterface> LastActor;
	TScriptInterface<IInteractableInterface> ThisActor;
	//void GetInteractionHovered();
	
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
	
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> InteractAction;

	// Movement Config
	UPROPERTY(EditAnywhere, Category = "Movement")
	float LeanSpeed = 5.f;
	// Input Handlers
	void Move(const FInputActionValue& InputActionValue);
	void Look(const FInputActionValue& InputActionValue);
	void JumpPressed(const FInputActionValue& InputActionValue);
	void JumpReleased(const FInputActionValue& InputActionValue);
	void CrouchPressed(const FInputActionValue& InputActionValue);
	void CrouchReleased(const FInputActionValue& InputActionValue);
	void Interact(const FInputActionValue& InputActionValue);

	// Advanced Movement 
	void MovementLean(float ScaleVal);
	void UpdateLean(float LeanRoll);

	// Checking Under Crosshair for Weapon
	void CrosshairTrace();

	
	

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UWWPickupWidgetBase> InteractionWidget;

	UWWPickupWidgetBase* CurrentInteractionWidget = nullptr;


	UPROPERTY(EditDefaultsOnly, Category = "UI|Interaction")
	TSubclassOf<UInteractionTextComponent> InteractionTextComponentClass;

	


	
};
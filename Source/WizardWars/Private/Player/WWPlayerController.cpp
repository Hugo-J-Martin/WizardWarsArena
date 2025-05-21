// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/WWPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Blueprint/UserWidget.h"
#include "Character/WWCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interaction/InteractableInterface.h"
#include "Widget/WWPickupWidgetBase.h"


AWWPlayerController::AWWPlayerController()
{
	bReplicates = true;
}
void AWWPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("This is a test log!"));

	// Only proceed on local player controller
	if (IsLocalController())
	{
		check(WWContext); // Still keep the check here for safety

		if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
			{
				Subsystem->AddMappingContext(WWContext, 0);
			}
		}

		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());
	}

	if (PickupWidgetClass)
	{
		PickupWidgetInstance = CreateWidget<UWWPickupWidgetBase>(this, PickupWidgetClass);
		UE_LOG(LogTemp, Warning, TEXT("CreatedWidget!"));
	}
	
}
void AWWPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	if (IsLocalController())
	{
		CrosshairTrace();
	}

}



void AWWPlayerController::CrosshairTrace()
{
	
	AWWCharacterBase* MyCharacter = Cast<AWWCharacterBase>(GetPawn());
	if (!MyCharacter) return;

	UCameraComponent* FPSCamera = MyCharacter->GetFPSCamera();
	
	FVector TraceStart = FPSCamera->GetComponentLocation();
	FVector ForwardVector = FPSCamera->GetForwardVector();
	FVector TraceEnd = TraceStart + (ForwardVector * 500.f);
	
	FHitResult CrosshairHit;
	bool bHit = GetWorld()->LineTraceSingleByChannel(CrosshairHit, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility);
	if (bHit)
	{
		if (AActor* HitActor = CrosshairHit.GetActor())
		{
			if (HitActor && HitActor->Implements<UInteractableInterface>())
			{
				FString PickupName = IInteractableInterface::Execute_GetPickupName(HitActor);;
			}
		}
	}


	LastActor = ThisActor;
	ThisActor = CrosshairHit.GetActor();

	/**
	 *  Line Trace from crosshair. There are several scenarios:
	 *  A. LastActor is null && ThisActor is null
	 *		-Do nothing
	 *	B. LastActor is null && ThisActor is valid
	 *		- Highlight ThisActor
	 *	C. LastActor is valid && ThisActor is null
	 *		-UnHighlight LastActor
	 *	D. Both actors are valid, but Last Actor != ThisActor
	 *		-UnHighlight LastActor, and Highlight ThisActor
	 *	E. Both actors are valid, and are the same actor
	 *		-Do nothing
	 */

	if (LastActor == nullptr)
	{
		if (ThisActor != nullptr)
		{
			// Case B
			ThisActor->HighlightActor(this);
			
		}
		else
		{
			// Case A: Do nothing
		}
	}
	else // LastActor is valid
	{
		if (ThisActor == nullptr)
		{
			// Case C
			LastActor->UnHighlightActor(this);
		}
		else // Both actors are valid
		{
			if (LastActor != ThisActor)
			{
				// Case D
				LastActor->UnHighlightActor(this);
				ThisActor->HighlightActor(this);
				
			}
			else
			{
				// Case E: Do nothing
			}
		}
	}
		
}

void AWWPlayerController::ShowPickupWidget()
{
	UWWPickupWidgetBase* PickupWidgetActual = CreateWidget<UWWPickupWidgetBase>(this, UWWPickupWidgetBase::StaticClass());
	PickupWidgetActual->AddToViewport();
	if (PickupWidgetInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("ActivatePickup!"));
		PickupWidgetInstance->AddToViewport();
	}
}

void AWWPlayerController::HidePickupWidget()
{
	
	if (PickupWidgetInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("DeactivatePickup!"));
		PickupWidgetInstance->RemoveFromViewport();
	}
}



void AWWPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (!IsLocalController()) return;
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AWWPlayerController::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AWWPlayerController::Look);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AWWPlayerController::JumpPressed);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AWWPlayerController::JumpReleased);
	EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &AWWPlayerController::CrouchPressed);
	EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &AWWPlayerController::CrouchReleased);
}



void AWWPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(Rotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
	MovementLean(InputAxisVector.X);
}
void AWWPlayerController::Look(const FInputActionValue& InputActionValue)
{
	const FVector2D LookAxis = InputActionValue.Get<FVector2D>();

	// Apply yaw (left/right) and pitch (up/down) input
	AddYawInput(-LookAxis.X);
	AddPitchInput(LookAxis.Y); // Negative Y to match standard FPS behavior
}

void AWWPlayerController::JumpPressed(const FInputActionValue& InputActionValue)
{
	if (ACharacter* MyCharacter = Cast<AWWCharacterBase>(GetPawn()))
	{
		MyCharacter->Jump();
	}
}

void AWWPlayerController::JumpReleased(const FInputActionValue& InputActionValue)
{
	if (ACharacter* MyCharacter = Cast<AWWCharacterBase>(GetPawn()))
	{
		MyCharacter->StopJumping();
	}
}

void AWWPlayerController::CrouchPressed(const FInputActionValue& InputActionValue)
{
	if (ACharacter* MyCharacter = Cast<AWWCharacterBase>(GetPawn()))
	{
		MyCharacter->Crouch();
		UE_LOG(LogTemp, Warning, TEXT("IsCrouched: %d"), MyCharacter->bIsCrouched);
	}
}

void AWWPlayerController::CrouchReleased(const FInputActionValue& InputActionValue)
{
	if (ACharacter* MyCharacter = Cast<AWWCharacterBase>(GetPawn()))
	{
		MyCharacter->UnCrouch();
		UE_LOG(LogTemp, Warning, TEXT("IsCrouched: %d"), MyCharacter->bIsCrouched);
	}
}

void AWWPlayerController::MovementLean(float ScaleVal)
{
	if (ACharacter* MyCharacter = Cast<AWWCharacterBase>(GetPawn()))
	{
		// Check 1: Is the character moving?
		FVector Velocity = MyCharacter->GetVelocity();
		bool bIsMoving = Velocity.Size() > 0.f;
		
		bool bIsOnGround = false;
		if (MyCharacter && MyCharacter->GetCharacterMovement())
		{
			bIsOnGround = !MyCharacter->GetCharacterMovement()->IsFalling();
		}

		// Check 3: Is ScaleVal not 0
		bool bHasInput = !FMath::IsNearlyZero(ScaleVal);

		// Combine all conditions
		if (bIsMoving && bIsOnGround && bHasInput)
		{
			// ✅ All conditions met — apply leaning logic here
			UE_LOG(LogTemp, Log, TEXT("Leaning triggered: %f"), ScaleVal);
			if (ScaleVal > 0.f)
			{
				UpdateLean(2.5);
			}
			else if (ScaleVal < 0.f)
			{
				UpdateLean(357.5);
			}
		}
		else
		{
			
			UpdateLean(0);
		}
	}
}

void AWWPlayerController::UpdateLean(float LeanRoll)
{
	float DeltaTime = GetWorld()->GetDeltaSeconds();

	FRotator CurrentRotation = GetControlRotation();

	FRotator TargetRotation = CurrentRotation;
	TargetRotation.Roll = LeanRoll;

	float InterpSpeed = LeanSpeed;
	FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, InterpSpeed);

	SetControlRotation(NewRotation);
}



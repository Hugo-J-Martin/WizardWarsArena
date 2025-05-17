// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "WWCharacterBase.generated.h"

UCLASS(Abstract)
class WIZARDWARS_API AWWCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AWWCharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Character")
	TObjectPtr<USkeletalMeshComponent> FPSMesh;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> TPSWeapon;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> FPSWeapon;

	//void MoveForward(float Value);
	//void MoveRight(float Value);
	//void LookUp(float Value);
	//void Turn(float Value);

private:

	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* FPSCamera;

public:
	//called to bind functionality to input
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	

};

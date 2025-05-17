// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/WWCharacterBase.h"

#include "Camera/CameraComponent.h"
#include "DataWrappers/ChaosVDParticleDataWrapper.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AWWCharacterBase::AWWCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;

	FPSCamera = CreateDefaultSubobject<UCameraComponent>("FPSCamera");
	FPSCamera->SetupAttachment(RootComponent);
	FPSCamera->ProjectionMode = ECameraProjectionMode::Perspective;
	FPSCamera->bUsePawnControlRotation = true;
	

	GetMesh()->bOwnerNoSee = true;

	FPSMesh = CreateDefaultSubobject<USkeletalMeshComponent>("FPSMesh");
	FPSMesh->SetupAttachment(FPSCamera);
	FPSMesh->bOnlyOwnerSee = true;
	FPSMesh->bCastDynamicShadow = false;
	FPSMesh->CastShadow = false;
	
	FPSWeapon = CreateDefaultSubobject<USkeletalMeshComponent>("FPSWeapon");
	FPSWeapon->SetupAttachment(FPSMesh, FName("FPSWeaponHandSocket"));
	FPSWeapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FPSWeapon->bOnlyOwnerSee = true;
	
	TPSWeapon = CreateDefaultSubobject<USkeletalMeshComponent>("TPSWeapon");
	TPSWeapon->SetupAttachment(GetMesh(), FName("TPSWeaponHandSocket"));
	TPSWeapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	TPSWeapon->bOwnerNoSee = true;
}

// Called when the game starts or when spawned
void AWWCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

/**void AWWCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AWWCharacterBase::MoveForward(float Value)
{
}

void AWWCharacterBase::MoveRight(float Value)
{
}

void AWWCharacterBase::LookUp(float Value)
{
}

void AWWCharacterBase::Turn(float Value)
{
}
**/





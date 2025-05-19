// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/WWCharacterBase.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "Weapon/WWGunBase.h"

// Sets default values
AWWCharacterBase::AWWCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//Character Movement Attributes
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;

	GetCharacterMovement()->GravityScale = 2.2f;
	GetCharacterMovement()->MaxWalkSpeed = 5000.f;
	GetCharacterMovement()->GroundFriction = 4.f;
	GetCharacterMovement()->MaxWalkSpeed = 1000.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 100.f;
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 1.f;

	FPSCamera = CreateDefaultSubobject<UCameraComponent>("FPSCamera");
	FPSCamera->SetupAttachment(GetMesh(), FName("Head"));
	FPSCamera->ProjectionMode = ECameraProjectionMode::Perspective;
	FPSCamera->bUsePawnControlRotation = true;
	

	//GetMesh()->bOwnerNoSee = true;
/**
	FPSMesh = CreateDefaultSubobject<USkeletalMeshComponent>("FPSMesh");
	FPSMesh->SetupAttachment(FPSCamera);
	FPSMesh->bOnlyOwnerSee = true;
	FPSMesh->bCastDynamicShadow = false;
	FPSMesh->CastShadow = false;
	
	FPSWeapon = CreateDefaultSubobject<USkeletalMeshComponent>("FPSWeapon");
	FPSWeapon->SetupAttachment(FPSMesh, FName("FPSWeaponHandSocket"));
	FPSWeapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FPSWeapon->bOnlyOwnerSee = true;
**/
	
	TPSWeapon = CreateDefaultSubobject<USkeletalMeshComponent>("TPSWeapon");
	TPSWeapon->SetupAttachment(GetMesh(), FName("TPSWeaponHandSocket"));
	TPSWeapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//TPSWeapon->bOwnerNoSee = true;
}

void AWWCharacterBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AWWCharacterBase, OverlappingWeapon, COND_OwnerOnly);
}



UAbilitySystemComponent* AWWCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}



// Called when the game starts or when spawned
void AWWCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}



void AWWCharacterBase::SetOverlappingWeapon(AWWGunBase* Weapon)
{
	if (OverlappingWeapon)
	{
		OverlappingWeapon->ShowPickupWidget(false);
	}
	OverlappingWeapon = Weapon;
	if (IsLocallyControlled())
	{
		if (OverlappingWeapon)
		{
			OverlappingWeapon->ShowPickupWidget(true);
		}
	}
}
void AWWCharacterBase::OnRep_OverlappingWeapon(AWWGunBase* LastWeapon)
{
	if (OverlappingWeapon)
	{
		OverlappingWeapon->ShowPickupWidget(true);
	}
	if (LastWeapon)
	{
		LastWeapon->ShowPickupWidget(false);
	}
}

void AWWCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
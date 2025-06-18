// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/WWCharacterBase.h"

#include "Camera/CameraComponent.h"
#include "Component/WWCombatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "Player/WWPlayerController.h"
#include "Player/WWPlayerState.h"
#include "UI/HUD/WWHUD.h"
#include "UI/WidgetController/HUDWidgetController.h"
#include "Weapon/WWGunBase.h"

// Sets default values
AWWCharacterBase::AWWCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//Character Movement Attributes
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;

	GetCharacterMovement()->GravityScale = 2.2f;
	GetCharacterMovement()->MaxAcceleration = 5000.f;
	GetCharacterMovement()->GroundFriction = 4.f;
	GetCharacterMovement()->MaxWalkSpeed = 1000.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 100.f;
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = .8f;

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
	
	//PlayerWeapon = CreateDefaultSubobject<USkeletalMeshComponent>("PlayerWeapon");
	//PlayerWeapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	//PlayerWeapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//TPSWeapon->bOwnerNoSee = true;

	Combat = CreateDefaultSubobject<UWWCombatComponent>(TEXT("CombatComponent"));
	Combat->SetIsReplicated(true);
}

void AWWCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Init ability actor info for the Server
	InitAbilityActorInfo();
}

void AWWCharacterBase::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Init ability actor info for the client
	InitAbilityActorInfo();
}

void AWWCharacterBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AWWCharacterBase, OverlappingWeapon, COND_OwnerOnly);
}

void AWWCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (Combat)
	{
		Combat->Character = this;
	}
}


UAbilitySystemComponent* AWWCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}


void AWWCharacterBase::SetEquippedWeapon(AWWGunBase* NewWeapon)
{
	EquippedWeapon = NewWeapon;

	// Inform HUD (if already initialized)
	if (AWWPlayerController* WWPlayerController = Cast<AWWPlayerController>(GetController()))
	{
		if (AWWHUD* WWHUD = Cast<AWWHUD>(WWPlayerController->GetHUD()))
		{
			//WWHUD->OnWeaponEquipped(NewWeapon); // <-- See Step 3
		}
	}
}

// Called when the game starts or when spawned
void AWWCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}


void AWWCharacterBase::InitAbilityActorInfo()
{
	AWWPlayerState* WWPlayerState = GetPlayerState<AWWPlayerState>();
	check(WWPlayerState);
	WWPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(WWPlayerState, this);
	AbilitySystemComponent = WWPlayerState->GetAbilitySystemComponent();
	AttributeSet = WWPlayerState->GetAttributeSet();

	if (AWWPlayerController* WWPlayerController = Cast<AWWPlayerController>(GetController()))
	{
		if (AWWHUD* WWHUD = Cast <AWWHUD>(WWPlayerController->GetHUD()))
		{
			WWHUD->InitOverlay(WWPlayerController, WWPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}
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

void AWWCharacterBase::Equip()
{
	UE_LOG(LogTemp, Warning, TEXT("Equip button pressed"));
	if (Combat)
	{
		if (HasAuthority())
		{
			AWWPlayerController* PC = Cast<AWWPlayerController>(GetController());
			if (PC && PC->ThisActor)
			{
				Combat->EquipWeapon(Cast<AWWGunBase>(PC->ThisActor.GetObject()));
				//OverlappingWeapon->ShowPickupWidget(false);
				//OverlappingWeapon->UnHighlightActor(GetController());
			}
		}
		else
		{
			ServerEquipButtonPressed();
		}
		
	}
}
void AWWCharacterBase::ServerEquipButtonPressed_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Equip from server"));
	if (Combat && OverlappingWeapon)
	{
		Combat->EquipWeapon(OverlappingWeapon);
	}
	/**
	if (Combat)
	{
		UE_LOG(LogTemp, Warning, TEXT("Equip from server, first step"));
		AWWPlayerController* PC = Cast<AWWPlayerController>(GetController());
		if (PC && PC->ThisActor)
		{
			UE_LOG(LogTemp, Warning, TEXT("Equip from server, actually equip"));
			Combat->EquipWeapon(Cast<AWWGunBase>(PC->ThisActor.GetObject()));
			//OverlappingWeapon->ShowPickupWidget(false);
			//OverlappingWeapon->UnHighlightActor(GetController());
		}
	}
	**/
}


void AWWCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}





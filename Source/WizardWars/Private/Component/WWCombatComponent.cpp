// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/WWCombatComponent.h"
#include "Weapon/WWGunBase.h"
#include "AbilitySystemComponent.h"
#include "Character/WWCharacter.h"
#include "Character/WWCharacterBase.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Net/UnrealNetwork.h"
#include "Player/WWPlayerController.h"
#include "UI/HUD/WWHUD.h"

// Sets default values for this component's properties
UWWCombatComponent::UWWCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;


}


// Called when the game starts
void UWWCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	InitializeASC();
	Character = Cast<AWWCharacterBase>(GetOwner());
	
}

void UWWCombatComponent::OnRep_CurrentWeapon()
{
	if (!Character || !Character->IsLocallyControlled() || !CurrentWeapon) return;

	// Ensure the ASC is valid
	UAbilitySystemComponent* WeaponASC = CurrentWeapon->GetAbilitySystemComponent();
	UAttributeSet* WeaponAttributes = CurrentWeapon->GetAttributeSet();

	if (!WeaponASC || !WeaponAttributes)
	{
		UE_LOG(LogTemp, Warning, TEXT("ASC or AttributeSet is null in OnRep_CurrentWeapon."));
		return;
	}

	// Optional: Init ability actor info if needed
	WeaponASC->InitAbilityActorInfo(CurrentWeapon, CurrentWeapon);
	CurrentWeapon->OnEquipped();

	// Call HUD Init
	if (AWWHUD* HUD = Cast<AWWHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
	{
		UE_LOG(LogTemp, Warning, TEXT("OnRep_CurrentWeapon: Initializing HUD for local client."));
		HUD->InitWeaponOverlay(WeaponASC, Cast<UWWGunAttributeSet>(WeaponAttributes), CurrentWeapon->WeaponIcon, CurrentWeapon->WeaponName);
		UE_LOG(LogTemp, Warning, TEXT("Calling InitWeaponOverlay for %s, in OnRep_CurrentWeapon"), *Character->GetName());
	}
}

void UWWCombatComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UWWCombatComponent, CurrentWeapon);
}


void UWWCombatComponent::InitializeASC()
{
	AActor* Owner = GetOwner();
	if (Owner)
	{
		AbilityComponent = Owner->FindComponentByClass<UAbilitySystemComponent>();
	}
}

// Called every frame
void UWWCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UWWCombatComponent::EquipWeapon(AWWGunBase* WeaponToEquip)
{
	if (Character == nullptr || WeaponToEquip == nullptr) return;

	if (CurrentWeapon) return;
	CurrentWeapon = WeaponToEquip;
	UE_LOG(LogTemp, Warning, TEXT("Calling InitWeaponOverlay for %s, in EquipWeapon"), *Character->GetName());
	//ADD WEAPON.EQUIPPED GAMEPLAYTAG HERE
	const USkeletalMeshSocket* HandSocket = Character->GetMesh()->GetSocketByName(FName("WeaponHandSocket"));
	if (HandSocket)
	{
		HandSocket->AttachActor(CurrentWeapon, Character->GetMesh());
	}
	AWWCharacterBase* PlayerCharacter = Cast<AWWCharacterBase>(Character);

	AbilitySystemComponent = CurrentWeapon->GetAbilitySystemComponent();
	WeaponAttributeSet = CurrentWeapon->GetAttributeSet();
	float Ammo = WeaponAttributeSet->GetAmmo();
	UE_LOG(LogTemp, Warning, TEXT("Current Weapon Stats, Ammo: %f"), Ammo)
	AbilitySystemComponent->InitAbilityActorInfo(CurrentWeapon, CurrentWeapon);
	CurrentWeapon->SetOwner(Character);
	CurrentWeapon->OnEquipped();
	
	if (Character->IsLocallyControlled())
	{
		// Server's local player or owning client: safe to init HUD here
		if (AWWPlayerController* WWPlayerController = Cast<AWWPlayerController>(Character->GetController()))
		{
			if (AWWHUD* HUD = Cast<AWWHUD>(WWPlayerController->GetHUD()))
			{
				if (AbilitySystemComponent && WeaponAttributeSet)
				{
					HUD->InitWeaponOverlay(AbilitySystemComponent, Cast<UWWGunAttributeSet>(WeaponAttributeSet), CurrentWeapon->WeaponIcon, CurrentWeapon->WeaponName);
				}
			}
		}
	}
	
}


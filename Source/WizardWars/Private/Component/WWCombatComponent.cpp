// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/WWCombatComponent.h"
#include "Weapon/WWGunBase.h"
#include "AbilitySystemComponent.h"
#include "Character/WWCharacter.h"
#include "Character/WWCharacterBase.h"
#include "Engine/SkeletalMeshSocket.h"

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

	// ...
}

void UWWCombatComponent::EquipWeapon(AWWGunBase* WeaponToEquip)
{
	if (Character == nullptr || WeaponToEquip == nullptr) return;

	if (CurrentWeapon) return;
	CurrentWeapon = WeaponToEquip;
	//ADD WEAPON.EQUIPPED GAMEPLAYTAG HERE
	const USkeletalMeshSocket* HandSocket = Character->GetMesh()->GetSocketByName(FName("WeaponHandSocket"));
	if (HandSocket)
	{
		HandSocket->AttachActor(CurrentWeapon, Character->GetMesh());
	}
	AWWCharacterBase* PlayerCharacter = Cast<AWWCharacterBase>(Character);
	if (Character)
	{
		PlayerCharacter->SetEquippedWeapon(CurrentWeapon);
	}
	CurrentWeapon->SetOwner(Character);
	//CurrentWeapon->ShowPickupWidget(false);
	CurrentWeapon->ApplyEffect(CurrentWeapon->GE_Equipped);
	CurrentWeapon->OnPickup();
	//CurrentWeapon->UnHighlightActor();
	
}


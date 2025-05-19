// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WWGunBase.h"

#include "Character/WWCharacterBase.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"

#include "Net/UnrealNetwork.h"

// Sets default values
AWWGunBase::AWWGunBase()
{

	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	//SetReplicatingMovement(true);
	
	
	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>("GunMesh");
	GunMesh->SetupAttachment(RootComponent);
	SetRootComponent(GunMesh);

	//Gun Collisions
	GunMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	GunMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	GunMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	

	PickupRadius = CreateDefaultSubobject<USphereComponent>("PickupRadius");
	PickupRadius->SetupAttachment(RootComponent);
	PickupRadius->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	PickupRadius->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//Radius Default values
	PickupRadius->SetSphereRadius(100.f);
	PickupRadius->SetVisibility(true);
	PickupRadius->SetHiddenInGame(false);

	//Pickup Widget
	PickupWidget = CreateDefaultSubobject<UWidgetComponent>("PickupWidget");
	PickupWidget->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AWWGunBase::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		PickupRadius->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		PickupRadius->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
		PickupRadius->OnComponentBeginOverlap.AddDynamic(this, &AWWGunBase::OnSphereOverlap);
		PickupRadius->OnComponentEndOverlap.AddDynamic(this, &AWWGunBase::OnSphereEndOverlap);
	}
	if (PickupWidget)
	{
		PickupWidget->SetVisibility(false);
	}
}
// Called every frame
void AWWGunBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void AWWGunBase::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AWWCharacterBase* WWCharacter = Cast<AWWCharacterBase>(OtherActor);
	if (WWCharacter)
	{
		WWCharacter->SetOverlappingWeapon(this);
	}
}

void AWWGunBase::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AWWCharacterBase* WWCharacter = Cast<AWWCharacterBase>(OtherActor);
	if (WWCharacter)
	{
		WWCharacter->SetOverlappingWeapon(nullptr);
	}
}


void AWWGunBase::ShowPickupWidget(bool bShowWidget)
{
	if (PickupWidget)
	{
		PickupWidget->SetVisibility(bShowWidget);
	}
}


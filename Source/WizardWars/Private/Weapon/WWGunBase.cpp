// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WWGunBase.h"

#include "Character/WWCharacterBase.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"

#include "Net/UnrealNetwork.h"
#include "Player/WWPlayerController.h"

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
	
	//Pickup Radius Initialize
	PickupRadius = CreateDefaultSubobject<USphereComponent>("PickupRadius");
	PickupRadius->SetupAttachment(RootComponent);
	PickupRadius->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	PickupRadius->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//Pickup Radius Default values
	PickupRadius->SetSphereRadius(300.f);
	PickupRadius->SetVisibility(true);
	PickupRadius->SetHiddenInGame(false);

	//Highlight Radius Initialize
	HighlightRadius = CreateDefaultSubobject<USphereComponent>("HighlightRadius");
	HighlightRadius->SetupAttachment(RootComponent);
	HighlightRadius->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	HighlightRadius->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

	//Pickup Radius Default values
	HighlightRadius->SetSphereRadius(100.f);
	HighlightRadius->SetVisibility(true);
	HighlightRadius->SetHiddenInGame(false);

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
void AWWGunBase::HighlightActor(AWWPlayerController* PC)
{
	bHighlighted = true;
	GunMesh->SetRenderCustomDepth(true);
	AWWPlayerController* WWPC = Cast<AWWPlayerController>(PC);
	WWPC->ShowPickupWidget();
}

void AWWGunBase::UnHighlightActor(AWWPlayerController* PC)
{
	bHighlighted = false;
	GunMesh->SetRenderCustomDepth(false);
	AWWPlayerController* WWPC = Cast<AWWPlayerController>(PC);
	WWPC->HidePickupWidget();
}

FString AWWGunBase::GetPickupName_Implementation() const
{
	return WeaponName;
}
void AWWGunBase::ShowPickupWidget(bool bShowWidget)
{
	if (PickupWidget)
	{
		PickupWidget->SetVisibility(bShowWidget);
	}
}






// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/WWPickupEffectActor.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/WWAttributeSet.h"
#include "Components/SphereComponent.h"

AWWPickupEffectActor::AWWPickupEffectActor()
{

	PrimaryActorTick.bCanEverTick = false;

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>("PickupMesh");
	SetRootComponent(PickupMesh);
	
	PickupRadius = CreateDefaultSubobject<USphereComponent>("PickupRadius");
	PickupRadius->SetupAttachment(GetRootComponent());
	

}

void AWWPickupEffectActor::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//TODO: Change this to apply a Gameplay Effect. For now, using const_cast as a hack, big no no
	if (IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(OtherActor))
	{
		const UWWAttributeSet* WWAttributeSet = Cast<UWWAttributeSet>(ASCInterface->GetAbilitySystemComponent()->GetAttributeSet(UWWAttributeSet::StaticClass()));

		UWWAttributeSet* MutableWWAttributeSet = const_cast<UWWAttributeSet*>(WWAttributeSet);
		MutableWWAttributeSet->SetHealth(WWAttributeSet->GetHealth() + 25.f);
		MutableWWAttributeSet->SetArmor(WWAttributeSet->GetArmor() + 5.f);
		Destroy();
	}
}

void AWWPickupEffectActor::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
}


void AWWPickupEffectActor::BeginPlay()
{
	Super::BeginPlay();

	PickupRadius->OnComponentBeginOverlap.AddDynamic(this, &AWWPickupEffectActor::OnSphereOverlap);
	PickupRadius->OnComponentEndOverlap.AddDynamic(this, &AWWPickupEffectActor::OnSphereEndOverlap);
}


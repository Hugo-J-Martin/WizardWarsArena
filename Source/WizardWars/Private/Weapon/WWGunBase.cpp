// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WWGunBase.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/WWGunAttributeSet.h"
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
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	
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
	PickupRadius->SetSphereRadius(1000.f);
	PickupRadius->SetVisibility(true);
	//PickupRadius->SetHiddenInGame(false);

	//Highlight Radius Initialize
	HighlightRadius = CreateDefaultSubobject<USphereComponent>("HighlightRadius");
	HighlightRadius->SetupAttachment(RootComponent);
	HighlightRadius->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	HighlightRadius->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);

	//Pickup Radius Default values
	HighlightRadius->SetSphereRadius(100.f);
	HighlightRadius->SetVisibility(true);
	//HighlightRadius->SetHiddenInGame(false);

	AbilitySystem = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystem"));
	AttributeSet = CreateDefaultSubobject<UWWGunAttributeSet>(TEXT("AttributeSet"));

	FActiveGameplayEffectHandle EquippedEffectHandle;

	
	//EquippedEffectHandle = AbilitySystemComponent->ApplyGameplayEffectToSelf(GE_ReadyToPickup->GetDefaultObject<UGameplayEffect>(), 1.f, AbilitySystemComponent->MakeEffectContext());
	
	
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
	if (HasAuthority() && AbilitySystem && AttributeSet)
	{
		UE_LOG(LogTemp, Warning, TEXT("Has AttributeSet"));
	}
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		ApplyEffect(GE_ReadyToPickup);
	}
}

void AWWGunBase::InitializeEffect()
{

	
}

void AWWGunBase::ApplyEffect(TSubclassOf<UGameplayEffect> EffectToApply)
{
	if (!EffectToApply) return;
	if (!AbilitySystemComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("AbilitySystemComponent is null in %s"), *GetName());
		return;
	}
	FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(EffectToApply, 1.f, EffectContextHandle);
	if (EffectSpecHandle.IsValid())
	{
		// 3. Apply the effect to self
		FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

		if (ActiveGEHandle.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("Gun's GameplayEffect: %s"), *EffectToApply->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to apply GameplayEffect"));
		}
	}
}
void AWWGunBase::RemoveEffect(TSubclassOf<UGameplayEffect> GameplayEffect)
{
	
}

void AWWGunBase::OnPickup()
{
	HighlightRadius->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	PickupRadius->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	//ShowPickupWidget(false);
	//bHighlighted = false;
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

void AWWGunBase::InitAbilityActorInfo()
{
	
}

void AWWGunBase::HighlightActor(AWWPlayerController* PC)
{
	if (CurrentGameplayEffect == GE_Equipped) return;
	bHighlighted = true;
	GunMesh->SetRenderCustomDepth(true);
}

void AWWGunBase::UnHighlightActor(AWWPlayerController* PC)
{
	bHighlighted = false;
	GunMesh->SetRenderCustomDepth(false);
}

FString AWWGunBase::GetPickupName_Implementation() const
{
	return WeaponName;
}

UAbilitySystemComponent* AWWGunBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AWWGunBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
}

void AWWGunBase::ShowPickupWidget(bool bShowWidget)
{
	if (CurrentGameplayEffect == GE_Equipped) return;
	UE_LOG(LogTemp, Warning, TEXT("Weapon can be shown: %s"), bShowWidget ? TEXT("true") : TEXT("false"));
	if (PickupWidget)
	{
		PickupWidget->SetVisibility(bShowWidget);
	}
}






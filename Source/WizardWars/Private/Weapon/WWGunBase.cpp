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
	SetReplicates(true);
	//SetReplicatingMovement(true);
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	AttributeSet = CreateDefaultSubobject<UWWGunAttributeSet>(TEXT("AttributeSet"));
	NetUpdateFrequency = 100.f;
	
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
	
	FActiveGameplayEffectHandle EquippedEffectHandle;
	
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

		//SetWeaponStateTag(InitialTag);
	}
	if (PickupWidget)
	{
		PickupWidget->SetVisibility(false);
	}
	if (HasAuthority() && AbilitySystemComponent && AttributeSet)
	{
		UE_LOG(LogTemp, Warning, TEXT("Has AttributeSet"));
	}
	if (AbilitySystemComponent)
	{
		
		ApplyWeaponEffect(GE_ReadyToPickup);
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

void AWWGunBase::ApplyWeaponEffect(TSubclassOf<UGameplayEffect> Effect)
{
	if (!HasAuthority() || !AbilitySystemComponent || !Effect) return;

	FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);

	FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(Effect, 1.f, EffectContextHandle);
	if (SpecHandle.IsValid())
	{
		ActiveWeaponEffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		if (!ActiveWeaponEffectHandle.IsValid())
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to apply gameplay effect %s"), *Effect->GetName());
		}
		
		CurrentGameplayEffect = Effect;

		//Local side effects here (UI, collision, fx)
		FGameplayTagContainer GrantedTags;
		//const FGameplayTagContainer& Tags = SpecHandle.Data->GetAllGrantedTags(); //Add param here
		SpecHandle.Data->GetAllGrantedTags(GrantedTags);

		//logging the effect
		UE_LOG(LogTemp, Log, TEXT("Applied Weapon GameplayEffect: %s with Tags: %s"),
		*Effect->GetName(),
		*GrantedTags.ToString());

		//logic to deal with on equipped and on dropped
		if (GrantedTags.HasTag(FGameplayTag::RequestGameplayTag(FName("Weapon.States.Equipped"))))
		{
			ShowPickupWidget(false);
			PickupRadius->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			HighlightRadius->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
		else if (GrantedTags.HasTag(FGameplayTag::RequestGameplayTag(FName("Weapon.States.Dropped"))))
		{
			ShowPickupWidget(true);
			PickupRadius->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			HighlightRadius->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		}
		
	}
}

void AWWGunBase::RemoveWeaponEffect(TSubclassOf<UGameplayEffect> Effect)
{
	if (!HasAuthority() || !AbilitySystemComponent || !Effect) return;

	// Default query with no filters matches all active effects
	FGameplayEffectQuery Query;

	// Get all active effect handles
	TArray<FActiveGameplayEffectHandle> ActiveHandles = AbilitySystemComponent->GetActiveEffects(Query);

	TArray<FActiveGameplayEffectHandle> HandlesToRemove;

	for (const FActiveGameplayEffectHandle& Handle : ActiveHandles)
	{
		const FActiveGameplayEffect* ActiveEffect = AbilitySystemComponent->GetActiveGameplayEffect(Handle);
		if (ActiveEffect && ActiveEffect->Spec.Def && ActiveEffect->Spec.Def->GetClass() == Effect)
		{
			HandlesToRemove.Add(Handle);
		}
	}

	for (const FActiveGameplayEffectHandle& Handle : HandlesToRemove)
	{
		AbilitySystemComponent->RemoveActiveGameplayEffect(Handle);
		UE_LOG(LogTemp, Log, TEXT("Removed GameplayEffect: %s"), *Effect->GetName());

		if (CurrentGameplayEffect == Effect)
		{
			CurrentGameplayEffect = nullptr;
			ActiveWeaponEffectHandle.Invalidate();
		}
	}

	if (HandlesToRemove.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No active instance of GameplayEffect %s was found."), *Effect->GetName());
	}
}


void AWWGunBase::OnEquipped()
{
	// Make sure this is only called on the server
	if (!HasAuthority()) return;

	AWWCharacterBase* CharacterOwner = Cast<AWWCharacterBase>(GetOwner());
	if (!CharacterOwner)
	{
		UE_LOG(LogTemp, Error, TEXT("Weapon %s equipped but owner is not a valid character"), *GetName());
		return;
	}
	AbilitySystemComponent->InitAbilityActorInfo(CharacterOwner, this);
	RemoveWeaponEffect(GE_ReadyToPickup);
	RemoveWeaponEffect(GE_Dropped);
	ApplyWeaponEffect(GE_Equipped);
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






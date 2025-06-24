// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AttributeSet.h"
#include "GameplayEffect.h"
#include "AbilitySystem/WWGunAttributeSet.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Actor.h"
#include "Interaction/InteractableInterface.h"
#include "WWGunBase.generated.h"

class UGameplayEffect;
class UAbilitySystemComponent;
class UAttributeSet;
UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	EWS_Initial UMETA(DisplayName = "Inital State"),
	EWS_Equipped UMETA(DisplayName = "Equipped"),
	EWS_Dropped UMETA(DisplayName = "Dropped"),
	EWS_MAX UMETA(DisplayName = "DefaultMAX")
};



UCLASS()
class WIZARDWARS_API AWWGunBase : public AActor, public IInteractableInterface, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWWGunBase();
	virtual void Tick(float DeltaTime) override;
	void ShowPickupWidget(bool bShowWidget);

	virtual void HighlightActor(AWWPlayerController* PC) override;
	virtual void UnHighlightActor(AWWPlayerController* PC) override;

	virtual FString GetPickupName_Implementation() const override;

	//virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//UAttributeSet* GetAttributeSet() const { return AttributeSet; }
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable)
	UAbilitySystemComponent* GetAbilitySystemComponent() const { return AbilitySystemComponent; }
	
	UFUNCTION(BlueprintCallable)
	UWWGunAttributeSet* GetAttributeSet() const { return AttributeSet; }

	UPROPERTY()
	UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	UWWGunAttributeSet* AttributeSet;

	//UPROPERTY(BlueprintReadOnly, Category = "Ammo", meta = (AllowPrivateAccess = true))
	//FGameplayAttributeData Ammo;

	//UPROPERTY(BlueprintReadOnly, Category = "Ammo", meta = (AllowPrivateAccess = true))
	//FGameplayAttributeData ReserveAmmo;
	

	virtual void InitializeEffect();
	virtual void ApplyEffect(TSubclassOf<UGameplayEffect> GameplayEffect);
	virtual void RemoveEffect(TSubclassOf<UGameplayEffect> GameplayEffect);

	//void SetWeaponStateTag(FGameplayTag NewStateTag);
	//FGameplayTag GetCurrentWeaponStateTag() const;

	void ApplyWeaponEffect(TSubclassOf<UGameplayEffect> Effect);
	void RemoveWeaponEffect(TSubclassOf<UGameplayEffect> Effect);
	
	virtual void OnEquipped();
	
	UPROPERTY(BlueprintReadOnly)
	FString PickupName;

	UPROPERTY(BlueprintReadOnly)
	bool bHighlighted = false;

	UPROPERTY(BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> CurrentGameplayEffect;

	UPROPERTY(EditAnywhere, Category = "Tags")
	TSubclassOf<UGameplayEffect> GE_Equipped;

	UPROPERTY(EditAnywhere, Category = "Tags")
	TSubclassOf<UGameplayEffect> GE_Dropped;

	UPROPERTY(EditAnywhere, Category = "Tags")
	TSubclassOf<UGameplayEffect> GE_ReadyToPickup;

	FActiveGameplayEffectHandle ActiveWeaponEffectHandle;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	FString WeaponName;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Properties")
	UTexture2D* WeaponIcon;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



	UFUNCTION()
	virtual void OnSphereOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
		);
	UFUNCTION()
	void OnSphereEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

	//void ClearWeaponStateTags();
	/**
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	**/
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	TObjectPtr<UAbilitySystemComponent> AbilitySystem;
	
	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	TObjectPtr<USkeletalMeshComponent> GunMesh;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties|Pickup")
	TObjectPtr<USphereComponent> PickupRadius;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties|Pickup")
	TObjectPtr<USphereComponent> HighlightRadius;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	EWeaponState WeaponState;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties|Pickup")
	TObjectPtr<UWidgetComponent> PickupWidget;

	

	//FGameplayTag InitialTag;
	//FGameplayTag EquippedTag;
	//FGameplayTag DroppedTag;


private:	
	
	void InitAbilityActorInfo();
	
};

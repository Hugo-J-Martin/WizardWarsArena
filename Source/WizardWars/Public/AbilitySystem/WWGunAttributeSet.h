// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "WWGunAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
/**
 * 
 */
UCLASS()
class WIZARDWARS_API UWWGunAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UWWGunAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Damage, Category = "Stats")
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UWWGunAttributeSet, Damage);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ReloadTime, Category = "Stats")
	FGameplayAttributeData ReloadTime;
	ATTRIBUTE_ACCESSORS(UWWGunAttributeSet, ReloadTime);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Ammo, Category = "Stats")
	FGameplayAttributeData Ammo;
	ATTRIBUTE_ACCESSORS(UWWGunAttributeSet, Ammo);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxAmmo, Category = "Stats")
	FGameplayAttributeData MaxAmmo;
	ATTRIBUTE_ACCESSORS(UWWGunAttributeSet, MaxAmmo);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ReserveAmmo, Category = "Stats")
	FGameplayAttributeData ReserveAmmo;
	ATTRIBUTE_ACCESSORS(UWWGunAttributeSet, ReserveAmmo);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_FireRate, Category = "Stats")
	FGameplayAttributeData FireRate;
	ATTRIBUTE_ACCESSORS(UWWGunAttributeSet, FireRate);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ProjectileSpeed, Category = "Stats")
	FGameplayAttributeData ProjectileSpeed;
	ATTRIBUTE_ACCESSORS(UWWGunAttributeSet, ProjectileSpeed);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Recoil, Category = "Stats")
	FGameplayAttributeData Recoil;
	ATTRIBUTE_ACCESSORS(UWWGunAttributeSet, Recoil);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BulletsPerShot, Category = "Stats")
	FGameplayAttributeData BulletsPerShot;
	ATTRIBUTE_ACCESSORS(UWWGunAttributeSet, BulletsPerShot);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Spread, Category = "Stats")
	FGameplayAttributeData Spread;
	ATTRIBUTE_ACCESSORS(UWWGunAttributeSet, Spread);

	UFUNCTION()
	void OnRep_Damage(const FGameplayAttributeData& OldDamage) const;
	UFUNCTION()
	void OnRep_ReloadTime(const FGameplayAttributeData& OldReloadTime) const;
	UFUNCTION()
	void OnRep_Ammo(const FGameplayAttributeData& OldAmmo) const;
	UFUNCTION()
	void OnRep_MaxAmmo(const FGameplayAttributeData& OldMaxAmmo) const;
	UFUNCTION()
	void OnRep_ReserveAmmo(const FGameplayAttributeData& OldReserveAmmo) const;
	UFUNCTION()
	void OnRep_FireRate(const FGameplayAttributeData& OldFireRate) const;
	UFUNCTION()
	void OnRep_ProjectileSpeed(const FGameplayAttributeData& OldProjectileSpeed) const;
	UFUNCTION()
	void OnRep_Recoil(const FGameplayAttributeData& OldRecoil) const;
	UFUNCTION()
	void OnRep_BulletsPerShot(const FGameplayAttributeData& OldBulletsPerShot) const;
	UFUNCTION()
	void OnRep_Spread(const FGameplayAttributeData& OldSpread) const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetDamageValue() const { return Damage.GetCurrentValue(); }
	
	
};

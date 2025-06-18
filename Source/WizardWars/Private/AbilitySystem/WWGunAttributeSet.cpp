// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/WWGunAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

UWWGunAttributeSet::UWWGunAttributeSet()
{
	InitDamage(20.f);
	InitReloadTime(1.5f);
	InitAmmo(30.f);
	InitMaxAmmo(30.f);
	InitReserveAmmo(60.f);
	InitFireRate(1.f);
	InitProjectileSpeed(50.f);
	InitRecoil(1.f);
	InitBulletsPerShot(1.f);
	InitSpread(1.f);
}

void UWWGunAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UWWGunAttributeSet, Damage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UWWGunAttributeSet, ReloadTime, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UWWGunAttributeSet, Ammo, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UWWGunAttributeSet, MaxAmmo, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UWWGunAttributeSet, ReserveAmmo, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UWWGunAttributeSet, FireRate, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UWWGunAttributeSet, ProjectileSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UWWGunAttributeSet, Recoil, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UWWGunAttributeSet, BulletsPerShot, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UWWGunAttributeSet, Spread, COND_None, REPNOTIFY_Always);
}

void UWWGunAttributeSet::OnRep_Damage(const FGameplayAttributeData& OldDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWWGunAttributeSet, Damage, OldDamage);
}

void UWWGunAttributeSet::OnRep_ReloadTime(const FGameplayAttributeData& OldReloadTime) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWWGunAttributeSet, ReloadTime, OldReloadTime);
}

void UWWGunAttributeSet::OnRep_Ammo(const FGameplayAttributeData& OldAmmo) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWWGunAttributeSet, Ammo, OldAmmo);
}

void UWWGunAttributeSet::OnRep_MaxAmmo(const FGameplayAttributeData& OldMaxAmmo) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWWGunAttributeSet, MaxAmmo, OldMaxAmmo);
}

void UWWGunAttributeSet::OnRep_ReserveAmmo(const FGameplayAttributeData& OldReserveAmmo) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWWGunAttributeSet, ReserveAmmo, OldReserveAmmo);
}

void UWWGunAttributeSet::OnRep_FireRate(const FGameplayAttributeData& OldFireRate) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWWGunAttributeSet, FireRate, OldFireRate);
}

void UWWGunAttributeSet::OnRep_ProjectileSpeed(const FGameplayAttributeData& OldProjectileSpeed) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWWGunAttributeSet, ProjectileSpeed, OldProjectileSpeed);
}

void UWWGunAttributeSet::OnRep_Recoil(const FGameplayAttributeData& OldRecoil) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWWGunAttributeSet, Recoil, OldRecoil);
}

void UWWGunAttributeSet::OnRep_BulletsPerShot(const FGameplayAttributeData& OldBulletsPerShot) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWWGunAttributeSet, BulletsPerShot, OldBulletsPerShot);
}

void UWWGunAttributeSet::OnRep_Spread(const FGameplayAttributeData& OldSpread) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWWGunAttributeSet, Spread, OldSpread);
}

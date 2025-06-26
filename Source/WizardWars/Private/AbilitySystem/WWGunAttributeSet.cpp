// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/WWGunAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "UI/WidgetController/HUDWeaponWidgetController.h"
#include "Weapon/WWGunBase.h"

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

void UWWGunAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	//Clamps Ammo to MaxAmmo
	if (Attribute == GetAmmoAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxAmmo());
		
	}
	if (Attribute == GetReserveAmmoAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, 999.f);
	}
	
}

void UWWGunAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FWeaponEffectProperties& Props) const
{
	//Source = causer of the effect, Target = target of the effect (owner of this AttributeSet)
	
	Props.EffectContextHandle = Data.EffectSpec.GetContext();
	
	Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	if (IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo.IsValid() && Props.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();
		Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();
		if (Props.SourceController == nullptr && Props.SourceAvatarActor != nullptr)
		{
			if (APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor))
			{
				Props.SourceController = Pawn->GetController();
			}
		}
		if (Props.SourceController)
		{
			Props.SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn());
		}
	}

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo.IsValid())
	{
		Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetActor = Cast<AWWGunBase>(Props.TargetAvatarActor);
		Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
	}
}

void UWWGunAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FWeaponEffectProperties Props;
	SetEffectProperties(Data, Props);

	const FGameplayAttribute& ModifiedAttribute = Data.EvaluatedData.Attribute;

	if (ModifiedAttribute == GetReserveAmmoAttribute())
	{
		UE_LOG(LogTemp, Log, TEXT("ReserveAmmo changed to: %f"), GetReserveAmmo());
	}
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


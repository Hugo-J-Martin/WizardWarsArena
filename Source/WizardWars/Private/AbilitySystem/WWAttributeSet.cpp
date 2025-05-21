// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/WWAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"

UWWAttributeSet::UWWAttributeSet()
{
	InitHealth(100.f);
	InitMaxHealth(100.f);
	InitArmor(0.f);
	InitMaxArmor(100.f);
	InitMoveSpeed(1000.f);
}

void UWWAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UWWAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UWWAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UWWAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UWWAttributeSet, MaxArmor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UWWAttributeSet, MoveSpeed, COND_None, REPNOTIFY_Always);
}

void UWWAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWWAttributeSet, Health, OldHealth);
}

void UWWAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWWAttributeSet, MaxHealth, OldMaxHealth);
}

void UWWAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWWAttributeSet, Armor, OldArmor);
}

void UWWAttributeSet::OnRep_MaxArmor(const FGameplayAttributeData& OldMaxArmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWWAttributeSet, Armor, OldMaxArmor);
}

void UWWAttributeSet::OnRep_MoveSpeed(const FGameplayAttributeData& OldMoveSpeed) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWWAttributeSet, MoveSpeed, OldMoveSpeed);
}

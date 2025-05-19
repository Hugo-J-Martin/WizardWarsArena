// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/WWPlayerState.h"

#include "AbilitySystem/WWAbilitySystemComponent.h"
#include "AbilitySystem/WWAttributeSet.h"

AWWPlayerState::AWWPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UWWAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	AttributeSet = CreateDefaultSubobject<UWWAttributeSet>("AttributeSet");
	
	NetUpdateFrequency = 100.f;
}

UAbilitySystemComponent* AWWPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

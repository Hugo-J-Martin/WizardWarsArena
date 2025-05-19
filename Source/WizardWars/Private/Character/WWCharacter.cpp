// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/WWCharacter.h"
#include "AbilitySystemComponent.h"
#include "Player/WWPlayerState.h"


AWWCharacter::AWWCharacter()
{
	
}

void AWWCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Init ability actor info for the Server
	InitAbilityActorInfo();
}

void AWWCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Init ability actor info for the client
	InitAbilityActorInfo();
}

void AWWCharacter::InitAbilityActorInfo()
{
	AWWPlayerState* WWPlayerState = GetPlayerState<AWWPlayerState>();
	check(WWPlayerState);
	WWPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(WWPlayerState, this);
	AbilitySystemComponent = WWPlayerState->GetAbilitySystemComponent();
	AttributeSet = WWPlayerState->GetAttributeSet();
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/WWPickupEffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/WWAttributeSet.h"


AWWPickupEffectActor::AWWPickupEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}

void AWWPickupEffectActor::BeginPlay()
{
	Super::BeginPlay();
}

void AWWPickupEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	//const FGameplayEffectData& GameplayEffectData FOR THE HEADER
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
    if (TargetASC == nullptr) return;
	/**
    if (GameplayEffectData.EffectClass == nullptr) return;

	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);

	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(
		GameplayEffectData.EffectClass, 1.f, EffectContextHandle);

	const FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(
		*EffectSpecHandle.Data.Get());

	const bool bIsInfinite =
		(EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite);
	if (bIsInfinite && (GameplayEffectData.RemovalPolicy != EEffectRemovalPolicy::DoNotRemove))
	{
		ActiveEffectHandles.Add(ActiveEffectHandle, TargetASC);
	}
	**/
	/**
	check(GameplayEffectClass);
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, 1.f, EffectContextHandle);
	const FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

	const bool bIsInfinite =  EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;
	if (bIsInfinite && InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		ActiveEffectHandles.Add(ActiveEffectHandle, TargetASC);
	}
	
	**/
}



void AWWPickupEffectActor::OnOverlap(AActor* TargetActor)
{

	
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (TargetASC == nullptr) return;

	for (auto& Effect : GameplayEffects)
	{
		if (Effect.ApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
		{
			check(Effect.GameplayEffectClass);
			FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
			EffectContextHandle.AddSourceObject(this);
			const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(Effect.GameplayEffectClass, 1.f, EffectContextHandle);
			Effect.ActiveGameplayEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
		}
	}
	/**
for (const FGameplayEffectData& EffectData : GameplayEffectDataList)
{
	if (EffectData.ApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, EffectData);
	}

	if (EffectData.EffectType == EEffectType::Infinite &&
		EffectData.RemovalPolicy == EEffectRemovalPolicy::RemoveOnOverlap)
	{
		RemoveEffectFromTarget(TargetActor);
	}
}
**/
		/**
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}
	**/
}

void AWWPickupEffectActor::OnEndOverlap(AActor* TargetActor)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (TargetASC == nullptr) return;

	for (auto& Effect : GameplayEffects)
	{
		if (Effect.ApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
		{
			check(Effect.GameplayEffectClass);
			FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
			EffectContextHandle.AddSourceObject(this);
			const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(Effect.GameplayEffectClass, 1.f, EffectContextHandle);
			Effect.ActiveGameplayEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
		}
		if (Effect.RemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
		{
			check(Effect.GameplayEffectClass);
			TargetASC->RemoveActiveGameplayEffect(Effect.ActiveGameplayEffectHandle);
		}
	}
	/**
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}
	//Removing active gameplay effect
	if (InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if (!IsValid(TargetASC)) return;

		TArray<FActiveGameplayEffectHandle> HandlesToRemove;
		for (TTuple<FActiveGameplayEffectHandle, UAbilitySystemComponent*> HandlePair : ActiveEffectHandles)
		{
			if (TargetASC == HandlePair.Value)
			{
				TargetASC->RemoveActiveGameplayEffect(HandlePair.Key, 1);
				HandlesToRemove.Add(HandlePair.Key);
			}
		}
		for (FActiveGameplayEffectHandle& Handle : HandlesToRemove)
		{
			ActiveEffectHandles.FindAndRemoveChecked(Handle);
		}
	}
	**/
}


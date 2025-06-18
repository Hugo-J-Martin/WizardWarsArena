// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WWCombatComponent.generated.h"

class AWWGunBase;
class UAbilitySystemComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WIZARDWARS_API UWWCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UWWCombatComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	friend class AWWCharacterBase;

	void EquipWeapon(AWWGunBase* WeaponToEquip);
	
protected:
	virtual void BeginPlay() override;


private:
	
	
	void DropWeapon();
	AWWGunBase* GetCurrentWeapon() const;

	void StartFire();
	void FireHitScan();
	void FireProjectile();
	void StopFire();
	void Reload();
	

	AWWCharacterBase* Character;
	UPROPERTY()
	AWWGunBase* CurrentWeapon;

	UPROPERTY()
	UAbilitySystemComponent* AbilityComponent;

	void InitializeASC();


	

		
};

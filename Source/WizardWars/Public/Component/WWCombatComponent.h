// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UI/WidgetController/HUDWidgetController.h"
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

	void SetWidgetController(UHUDWidgetController* InWidgetController)
	{
		WidgetController = InWidgetController;
		UE_LOG(LogTemp, Warning, TEXT("CombatComponent: WidgetController Set"));
	}
	
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnRep_CurrentWeapon();

	void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
private:
	
	
	void DropWeapon();
	AWWGunBase* GetCurrentWeapon() const;

	void StartFire();
	void FireHitScan();
	void FireProjectile();
	void StopFire();
	void Reload();
	

	AWWCharacterBase* Character;
	
	UPROPERTY(ReplicatedUsing = OnRep_CurrentWeapon)
	AWWGunBase* CurrentWeapon;

	UPROPERTY()
	UAbilitySystemComponent* AbilityComponent;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UWWGunAttributeSet> WeaponAttributeSet;

	void InitializeASC();

	UPROPERTY()
	UHUDWidgetController* WidgetController;


	//FOnWeaponEquipped OnWeaponEquipped;


	

		
};

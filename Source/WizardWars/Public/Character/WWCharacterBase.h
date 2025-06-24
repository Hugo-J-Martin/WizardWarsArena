// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "Weapon/WWGunBase.h"
#include "WWCharacterBase.generated.h"


class UAbilitySystemComponent;
class UAttributeSet;

UCLASS(Abstract)
class WIZARDWARS_API AWWCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AWWCharacterBase();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	
	void Equip();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

	UPROPERTY(EditAnywhere, Category = "Character")
	TObjectPtr<USkeletalMeshComponent> FPSMesh;

	//UPROPERTY(EditAnywhere, Category = "Combat")
	//TObjectPtr<USkeletalMeshComponent> PlayerWeapon;

	//UPROPERTY(EditAnywhere, Category = "Combat")
	//TObjectPtr<USkeletalMeshComponent> FPSWeapon;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;





private:

	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* FPSCamera;
	
	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon)
	class AWWGunBase* OverlappingWeapon;

	UFUNCTION()
	void OnRep_OverlappingWeapon(AWWGunBase* LastWeapon);

	UPROPERTY(VisibleAnywhere)
	class UWWCombatComponent* Combat;

	void InitAbilityActorInfo();

	UFUNCTION(Server, Reliable)
	void ServerEquipButtonPressed();
	
public:
	
	void SetOverlappingWeapon(AWWGunBase* Weapon);
	UCameraComponent* GetFPSCamera() const {return FPSCamera;}
};
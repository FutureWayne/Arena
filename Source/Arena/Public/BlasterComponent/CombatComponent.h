// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"


class AWeapon;
class ABlasterCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARENA_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCombatComponent();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	friend class ABlasterCharacter;

	void EquipWeapon(AWeapon* WeaponToEquip);

	void DropWeapon();
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	void SetAiming(bool bNewAiming);

	UFUNCTION(Server, Reliable)
	void ServerSetAiming(bool bNewAiming);

	UFUNCTION()
	void OnRep_EquippedWeapon();

private:
	TObjectPtr<ABlasterCharacter> OwningCharacter;

	UPROPERTY(Replicated, ReplicatedUsing = OnRep_EquippedWeapon)
	TObjectPtr<AWeapon> EquippedWeapon;

	UPROPERTY(Replicated)
	bool bIsAiming;
};

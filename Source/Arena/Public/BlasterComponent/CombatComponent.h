// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

#define TRACE_LENGTH 80000.f

class ABlasterHUD;
class ABlasterPlayerController;
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
	
	void FireButtonPressed(bool bPressed);

	void SetHUDCrosshair(float DeltaTime);

	UFUNCTION(Server, Reliable)
	void ServerSetAiming(bool bNewAiming);

	UFUNCTION()
	void OnRep_EquippedWeapon() const;

	UFUNCTION(Server, Reliable)
	void ServerFire(const FVector_NetQuantize& TraceHitTarget);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastFire(const FVector_NetQuantize& TraceHitTarget);

private:
	bool bFireButtonPressed;

	float CrosshairVelocityFactor = 0.f;
	float CrosshairInAirFactor = 0.f;

	FVector HitTarget;
	
	TObjectPtr<ABlasterCharacter> OwningCharacter;
	TObjectPtr<ABlasterPlayerController> OwningController;
	TObjectPtr<ABlasterHUD> OwningHUD;

	UPROPERTY(Replicated, ReplicatedUsing = OnRep_EquippedWeapon)
	TObjectPtr<AWeapon> EquippedWeapon;

	UPROPERTY(Replicated)
	bool bIsAiming;

	void TraceUnderCrosshair(FHitResult& HitResult) const;
};

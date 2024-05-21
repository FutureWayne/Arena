// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterComponent/CombatComponent.h"

#include "Character/BlasterCharacter.h"
#include "Components/SphereComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "Weapon/Weapon.h"

// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UCombatComponent::SetAiming(const bool bNewAiming)
{
	bIsAiming = bNewAiming;
	ServerSetAiming(bNewAiming);
}

void UCombatComponent::OnRep_EquippedWeapon()
{
	if (EquippedWeapon && OwningCharacter)
	{
		OwningCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
	}
}

void UCombatComponent::ServerSetAiming_Implementation(const bool bNewAiming)
{
	bIsAiming = bNewAiming;
}

// Called every frame
void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCombatComponent, EquippedWeapon);
	DOREPLIFETIME(UCombatComponent, bIsAiming);
}

void UCombatComponent::EquipWeapon(AWeapon* WeaponToEquip)
{
	if (OwningCharacter == nullptr || WeaponToEquip == nullptr)
	{
		return;
	}

	EquippedWeapon = WeaponToEquip;
	EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);
	EquippedWeapon->GetWeaponMesh()->SetSimulatePhysics(false);
	EquippedWeapon->GetWeaponMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	EquippedWeapon->GetAreaSphere()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	EquippedWeapon->ShowPickupWidget(false);
	EquippedWeapon->SetOwner(OwningCharacter);
	
	if (const USkeletalMeshSocket* WeaponSocket = OwningCharacter->GetMesh()->GetSocketByName(FName("weapon_r")))
	{
		WeaponSocket->AttachActor(EquippedWeapon, OwningCharacter->GetMesh());
	}
	
	OwningCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
}

void UCombatComponent::DropWeapon()
{
	if (EquippedWeapon == nullptr)
	{
		return;
	}

	OwningCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
	
	EquippedWeapon->SetWeaponState(EWeaponState::EWS_Dropped);
	EquippedWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	EquippedWeapon->GetAreaSphere()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	EquippedWeapon->GetWeaponMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	EquippedWeapon->GetWeaponMesh()->SetSimulatePhysics(true);

	// Add a force to the weapon, using the character's aim rotation
	const FVector ForceVector = OwningCharacter->GetBaseAimRotation().Vector() * 400.f;
	EquippedWeapon->GetWeaponMesh()->AddImpulse(ForceVector, NAME_None, true);
	EquippedWeapon->SetOwner(nullptr);
	EquippedWeapon = nullptr;
}


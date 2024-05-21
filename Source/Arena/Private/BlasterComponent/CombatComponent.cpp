// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterComponent/CombatComponent.h"

#include "Character/BlasterCharacter.h"
#include "Components/SphereComponent.h"
#include "Engine/SkeletalMeshSocket.h"
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


// Called every frame
void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCombatComponent::EquipWeapon(AWeapon* WeaponToEquip)
{
	if (OwningCharacter == nullptr || WeaponToEquip == nullptr)
	{
		return;
	}

	EquippedWeapon = WeaponToEquip;
	EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);

	if (const USkeletalMeshSocket* WeaponSocket = OwningCharacter->GetMesh()->GetSocketByName("Weapon_R"))
	{
		WeaponSocket->AttachActor(EquippedWeapon, OwningCharacter->GetMesh());
	}

	EquippedWeapon->GetAreaSphere()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	EquippedWeapon->ShowPickupWidget(false);
	EquippedWeapon->SetOwner(OwningCharacter);
}

void UCombatComponent::DropWeapon()
{
	if (EquippedWeapon == nullptr)
	{
		return;
	}

	// TODO: Add physics simulation to the weapon
	
	EquippedWeapon->SetWeaponState(EWeaponState::EWS_Dropped);
	EquippedWeapon->GetAreaSphere()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	EquippedWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	EquippedWeapon->SetOwner(nullptr);
	EquippedWeapon = nullptr;
}


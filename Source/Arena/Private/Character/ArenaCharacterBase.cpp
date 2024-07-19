// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ArenaCharacterBase.h"

// Sets default values
AArenaCharacterBase::AArenaCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

UAbilitySystemComponent* AArenaCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UArenaHealthSet* AArenaCharacterBase::GetArenaHealthSet() const
{
	return ArenaHealthSet;
}

// Called when the game starts or when spawned
void AArenaCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AArenaCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AArenaCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


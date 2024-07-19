// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ArenaCharacter.h"

#include "AbilitySystemComponent.h"
#include "Player/ArenaPlayerState.h"

AArenaCharacter::AArenaCharacter()
{
}

void AArenaCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Init ability actor info for the server
	InitAbilityActorInfo();
}

void AArenaCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Init ability actor info for the client  
	InitAbilityActorInfo();
}

void AArenaCharacter::InitAbilityActorInfo()
{
	AArenaPlayerState* PS = GetPlayerState<AArenaPlayerState>();
	check(PS);

	// PlayerState logically owns the AbilitySystemComponent and AttributeSet while the Character is the physical actor
	PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);
	AbilitySystemComponent = PS->GetAbilitySystemComponent();
	ArenaHealthSet = PS->GetArenaHealthSet();
}

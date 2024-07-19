// Copyright Ludens Studio. All Rights Reserved.


#include "AbilitySystem/ArenaHealthSet.h"
#include "AbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"

UArenaHealthSet::UArenaHealthSet()
	: Health(100.0f)
	, MaxHealth(100.0f)
{
}

void UArenaHealthSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UArenaHealthSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UArenaHealthSet, MaxHealth, COND_None, REPNOTIFY_Always);
}

void UArenaHealthSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UArenaHealthSet, Health, OldHealth);
}

void UArenaHealthSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UArenaHealthSet, MaxHealth, OldMaxHealth);
}


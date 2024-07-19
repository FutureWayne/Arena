// Copyright Ludens Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/ArenaAttributeSet.h"
#include "ArenaHealthSet.generated.h"

/**
 * 
 */
UCLASS()
class ARENA_API UArenaHealthSet : public UArenaAttributeSet
{
	GENERATED_BODY()

public:
	UArenaHealthSet();

	ATTRIBUTE_ACCESSORS(UArenaHealthSet, Health);
	ATTRIBUTE_ACCESSORS(UArenaHealthSet, MaxHealth);

protected:
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;

private:

	// The current health attribute.  The health will be capped by the max health attribute.  Health is hidden from modifiers so only executions can modify it.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Lyra|Health", Meta = (HideFromModifiers, AllowPrivateAccess = true))
	FGameplayAttributeData Health;

	// The current max health attribute.  Max health is an attribute since gameplay effects can modify it.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Lyra|Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth;
	
};

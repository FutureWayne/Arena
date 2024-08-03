// Copyright Ludens Studio. All Rights Reserved.


#include "AbilitySystem/ArenaHealthSet.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/ArenaAbilitySystemComponent.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ArenaHealthSet)

UArenaHealthSet::UArenaHealthSet()
	: Health(100.0f)
	, MaxHealth(100.0f)
{
	bOutOfHealth = false;
	MaxHealthBeforeAttributeChange = 0.0f;
	HealthBeforeAttributeChange = 0.0f;
}

void UArenaHealthSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UArenaHealthSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UArenaHealthSet, MaxHealth, COND_None, REPNOTIFY_Always);
}

void UArenaHealthSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UArenaHealthSet, Health, OldHealth);

	const float CurrentHealth = GetHealth();
	const float EstimatedMagnitude = CurrentHealth - OldHealth.GetCurrentValue();

	OnHealthChanged.Broadcast(nullptr, nullptr, nullptr, EstimatedMagnitude, OldHealth.GetCurrentValue(), CurrentHealth);

	if (!bOutOfHealth && (CurrentHealth <= 0.0f))
	{
		OnOutOfHealth.Broadcast(nullptr, nullptr, nullptr, EstimatedMagnitude, OldHealth.GetCurrentValue(), CurrentHealth);
		bOutOfHealth = true;
	}
	
	bOutOfHealth = (CurrentHealth <= 0.0f);
}

void UArenaHealthSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UArenaHealthSet, MaxHealth, OldMaxHealth);

	OnMaxHealthChanged.Broadcast(nullptr, nullptr, nullptr, 0.0f, OldMaxHealth.GetCurrentValue(), GetMaxHealth());
}

bool UArenaHealthSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	if (!Super::PreGameplayEffectExecute(Data))
	{
		return false;
	}

	// Handle modifying incoming normal damage
	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		if (Data.EvaluatedData.Magnitude > 0.0f)
		{
			// TODO: Handle Cheat and Damage Immunity tags
		}
	}

	// Save the current health
	HealthBeforeAttributeChange = GetHealth();
	MaxHealthBeforeAttributeChange = GetMaxHealth();

	return true;
}

void UArenaHealthSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	const FGameplayEffectContextHandle& EffectContext = Data.EffectSpec.GetEffectContext();
	AActor* Instigator = EffectContext.GetOriginalInstigator();
	AActor* Causer = EffectContext.GetEffectCauser();

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		// Convert into -Health and then clamp
		SetHealth(FMath::Clamp(GetHealth() - GetDamage(), 0, GetMaxHealth()));
		SetDamage(0.0f);
	}

	else if (Data.EvaluatedData.Attribute == GetHealingAttribute())
	{
		// Convert into +Health and then clamp
		SetHealth(FMath::Clamp(GetHealth() + GetHealing(), 0, GetMaxHealth()));
		SetHealing(0.0f);
	}

	else if (Data.EvaluatedData.Attribute == GetMaxHealthAttribute())
	{
		// Clamp health to new max
		SetHealth(FMath::Clamp(GetHealth(), 0, GetMaxHealth()));
		OnMaxHealthChanged.Broadcast(Instigator, Causer, nullptr, GetMaxHealth() - MaxHealthBeforeAttributeChange, MaxHealthBeforeAttributeChange, GetMaxHealth());
	}

	if (GetHealth() != HealthBeforeAttributeChange)
	{
		OnHealthChanged.Broadcast(Instigator, Causer, &Data.EffectSpec, Data.EvaluatedData.Magnitude, HealthBeforeAttributeChange, GetHealth());
	}

	if ((GetHealth() <= 0.0f) && !bOutOfHealth)
	{
		OnOutOfHealth.Broadcast(Instigator, Causer, &Data.EffectSpec, Data.EvaluatedData.Magnitude, HealthBeforeAttributeChange, GetHealth());
	}

	bOutOfHealth = (GetHealth() <= 0.0f);
}

void UArenaHealthSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	ClampAttribute(Attribute, NewValue);
}

void UArenaHealthSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	ClampAttribute(Attribute, NewValue);
}

void UArenaHealthSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetMaxHealthAttribute())
	{
		if (GetHealth() > NewValue)
		{
			UArenaAbilitySystemComponent* ArenaASC = GetArenaAbilitySystemComponent();
			check(ArenaASC);

			ArenaASC->ApplyModToAttribute(GetHealthAttribute(), EGameplayModOp::Override, NewValue);
		}
	}

	if (bOutOfHealth && (GetHealth() > 0.0f))
	{
		bOutOfHealth = false;
	}
}

void UArenaHealthSet::ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}

	else if (Attribute == GetMaxHealthAttribute())
	{
		NewValue = FMath::Max(NewValue, 1.0f);
	}
}


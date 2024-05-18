// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BlasterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ARENA_API UBlasterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	UPROPERTY(BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = true))
	TObjectPtr<class ABlasterCharacter> BlasterCharacter;

	UPROPERTY(BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = true))
	float Speed = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = true))
	bool bIsInAir = false;

	UPROPERTY(BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = true))
	bool bIsAccelerating = false;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Character/NetProCharacterBase.h"
#include "NetProAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class NETWORKPROJECT_API UNetProAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UNetProAnimInstance(const FObjectInitializer& ObjectInitializer);
protected:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(BlueprintReadOnly)
	float Speed;
	UPROPERTY(BlueprintReadOnly)
	uint8 bSprinting:1;
	UPROPERTY(BlueprintReadOnly)
	uint8 bJump:1;
	
	UPROPERTY()
	ANetProCharacterBase* MyCharacter;
	
};

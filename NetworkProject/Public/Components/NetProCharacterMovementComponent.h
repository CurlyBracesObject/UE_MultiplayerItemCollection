// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NetProCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class NETWORKPROJECT_API UNetProCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
public:
	UNetProCharacterMovementComponent();
protected:
	virtual float GetMaxSpeed() const override;
protected:
	UPROPERTY(Category="Character Movement: Walking",EditAnywhere,BlueprintReadWrite)
	float SprintSpeed;
	
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/NetProCharacterMovementComponent.h"

#include "Character/NetProCharacterBase.h"

UNetProCharacterMovementComponent::UNetProCharacterMovementComponent()
{
	SprintSpeed=1000.f;
}


float UNetProCharacterMovementComponent::GetMaxSpeed() const
{
	float Speed=Super::GetMaxSpeed();
	if(ANetProCharacterBase* CharacterBase=Cast<ANetProCharacterBase>(GetOwner()))
	{
		if(CharacterBase->IsSprinting())
		{
			Speed=SprintSpeed;
		}
	}
	return Speed;
}

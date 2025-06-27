// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/NetProAnimInstance.h"

#include "GameFramework/PawnMovementComponent.h"

UNetProAnimInstance::UNetProAnimInstance(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	
}

void UNetProAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if(!MyCharacter)
	{
		MyCharacter=Cast<ANetProCharacterBase>(TryGetPawnOwner());
		return;
	}
	Speed=MyCharacter->GetVelocity().Size2D();
	bSprinting=MyCharacter->IsSprinting()&&Speed>0;
	bJump=MyCharacter->GetMovementComponent()->IsFalling();
}

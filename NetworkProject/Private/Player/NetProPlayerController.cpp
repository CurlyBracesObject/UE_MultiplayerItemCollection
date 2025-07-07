// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/NetProPlayerController.h"

#include "NetProGameState.h"

ANetProPlayerController::ANetProPlayerController()
{
	SetShowMouseCursor(true);
}

void ANetProPlayerController::PurchaseItem(int32 ItemID)
{
	if(HasAuthority())
	{
		if(ANetProGameState* MyGameState=GetWorld()->GetGameState<ANetProGameState>())
		{
			MyGameState->PurchaseItem(ItemID);
		}
	}else
	{
		Server_PurchaseItem(ItemID);
	}
}

void ANetProPlayerController::Server_PurchaseItem_Implementation(int32 ItemID)
{
	PurchaseItem(ItemID);
}

bool ANetProPlayerController::Server_PurchaseItem_Validate(int32 ItemID)
{
	return true;
}

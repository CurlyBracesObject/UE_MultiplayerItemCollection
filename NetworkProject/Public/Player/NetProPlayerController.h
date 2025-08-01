// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "NetProPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class NETWORKPROJECT_API ANetProPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ANetProPlayerController();

	void PurchaseItem(int32 ItemID);

	UFUNCTION(Server,Reliable,WithValidation)
	void Server_PurchaseItem(int32 ItemID);
	
};

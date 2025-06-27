// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "NetProPlayerStart.generated.h"

/**
 * 
 */
UCLASS()
class NETWORKPROJECT_API ANetProPlayerStart : public APlayerStart
{
	GENERATED_BODY()
public:
	ANetProPlayerStart(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere,Category="PlayerID")
	int32 AssignedPlayerID;
};

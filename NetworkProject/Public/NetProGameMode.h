// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "NetProGameMode.generated.h"

/**
 * 
 */
UCLASS()
class NETWORKPROJECT_API ANetProGameMode : public AGameMode
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;
	
public:
	ANetProGameMode();

	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
};

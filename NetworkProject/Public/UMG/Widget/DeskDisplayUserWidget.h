// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Player/NetProPlayerCharacter.h"
#include "DeskDisplayUserWidget.generated.h"

class UButton;
class UTextBlock;
/**
 * 
 */
UCLASS()
class NETWORKPROJECT_API UDeskDisplayUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	virtual void NativeConstruct() override;
	
	UFUNCTION()
	UPackageComponent* GetPlayerPackageComponent();

	void SetDeskLocation(FVector OutDeskLocation){DeskLocation=OutDeskLocation;}

	void UpdateScoreDisplay();
	
protected:
	
	UFUNCTION()
	void OnPlaceItemsButtonClicked();

	void OnScoreChanged(int32 PlayerID,int32 Score);

public:
	//ANetProPlayerCharacter* PlayerCharacter;
	
protected:
	UPROPERTY(meta=(BindWidget))
	UButton* PlaceItemsButton;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* ScoreText;
	
	FVector DeskLocation;
};

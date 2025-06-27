// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainGameUserWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class NETWORKPROJECT_API UMainGameUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void ShowGameResult(int32 WinnerPlayerID);
	
protected:
	virtual void NativeConstruct() override;
protected:
	UPROPERTY(meta=(BindWidget))
	UTextBlock* CountDownText;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* WinnerText;
	

	UFUNCTION()
	void UpdateCountDownText(int32 RemainingTime);
};

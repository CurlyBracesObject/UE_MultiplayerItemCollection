// Fill out your copyright notice in the Description page of Project Settings.


#include "UMG/MainGameUserWidget.h"

#include "NetProGameState.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"




void UMainGameUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if(ANetProGameState* GameState=GetWorld()->GetGameState<ANetProGameState>())
	{
		GameState->OnTimeChanged.AddUObject(this,&UMainGameUserWidget::UpdateCountDownText);
		GameState->OnGameResult.AddUObject(this,&UMainGameUserWidget::ShowGameResult);
	}
	if(WinnerText)
	{
		WinnerText->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UMainGameUserWidget::UpdateCountDownText(int32 RemainingTime)
{
	if(CountDownText)
	{
		CountDownText->SetText(FText::FromString(FString::Printf(TEXT("%d"),RemainingTime)));
	}
}

void UMainGameUserWidget::ShowGameResult(int32 WinnerPlayerID)
{
	if(WinnerText)
	{
		FString ResultText;
		if(WinnerPlayerID==0)
		{
			ResultText=TEXT("平局");
		}else
		{
			ResultText=FString::Printf(TEXT("玩家%d获胜"),WinnerPlayerID);
		}
		WinnerText->SetText(FText::FromString(ResultText));
		WinnerText->SetVisibility(ESlateVisibility::Visible);
	}
}
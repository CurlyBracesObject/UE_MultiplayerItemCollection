// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "NetProGameState.generated.h"


DECLARE_MULTICAST_DELEGATE_TwoParams(FOnScoreChanged,int32,int32);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnTimeChanged,int32);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnGameResult,int32);
		//MainGameUserWidget的ShowGameResult()在监听

//DECLARE_MULTICAST_DELEGATE_OneParam(FOnPurchasedItemChanged,const TArray<int32>&);
		//USingleItemInShop的OnPurchasedItemUpdated()监听

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPurchasedItemChanged,int32);
		//UShopItemsScrollBoxWidget的OnItemPurchasedGlobally(int32 ItemID)监听 移除已购买的商品UI
/**
 * 
 */
UCLASS()
class NETWORKPROJECT_API ANetProGameState : public AGameState
{
	GENERATED_BODY()

public:
	ANetProGameState();
	
	void AddScoreToPlayer(int32 PlayerID,int32 Score=1);

	int32 GetPlayerScore(int32 PlayerID) const;

	void CheckWinner();

	void StartGameCountDown(int32 Seconds);

	void PurchaseItem(int32 ItemID);
	
	bool IsItemPurchased(int32 ItemID) const;
	
protected:
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION(Server,Reliable,WithValidation)
	void Server_AddScoreToPlayer(int32 PlayerID,int32 Score=1);

	UFUNCTION(NetMulticast,Reliable)
	void Multi_ShowWinnerBroadcast(int32 WinnerPlayerID);

	/*UFUNCTION(Server,Reliable,WithValidation)
	void Server_PurchaseItem(int32 ItemID);*/

	/*UFUNCTION()
	void OnRep_PurchasedItems();*/

	UFUNCTION(NetMulticast,Reliable)
	void Multi_OnItemPurchased(int32 ItemID);
	
	

	UFUNCTION()
	void OnRep_PlayerOneScore();
	
	UFUNCTION()
	void OnRep_PlayerTwoScore();

	UFUNCTION()
	void OnRep_RemainingTime();

	UFUNCTION(NetMulticast,Reliable)
	void Multi_DisablePlayerInput();
	
public:
	/*UPROPERTY(Replicated,BlueprintReadOnly)
	int32 PlayerOneScore;*/
	UPROPERTY(ReplicatedUsing=OnRep_PlayerOneScore,BlueprintReadOnly)
	int32 PlayerOneScore;
	UPROPERTY(ReplicatedUsing=OnRep_PlayerTwoScore,BlueprintReadOnly)//ReplicatedUsing 只在客户端触发。
	//服务器改变后，同步到客户端时触发
	int32 PlayerTwoScore;

	UPROPERTY(ReplicatedUsing=OnRep_RemainingTime)
	int32 RemainingTime;

	UPROPERTY(Replicated)
	//UPROPERTY(ReplicatedUsing=OnRep_PurchasedItems)
	TArray<int32> PurchasedItemsArray;

	
	FOnScoreChanged OnScoreChanged;
	FOnTimeChanged OnTimeChanged;
	FOnGameResult OnGameResult;
		//void ANetProGameState::ShowWinner(int32 WinnerPlayerID)广播
			//MainGameUserWidget里初始化的时候UMainGameUserWidget::ShowGameResult监听
	
	//FOnPurchasedItemChanged OnPurchasedItemChanged;
	FOnPurchasedItemChanged OnPurchasedItemChanged;
	
protected:
	FTimerHandle CountDownTimer;
};

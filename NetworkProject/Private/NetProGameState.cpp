// Fill out your copyright notice in the Description page of Project Settings.


#include "NetProGameState.h"

#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

ANetProGameState::ANetProGameState()
{
	PlayerOneScore=0;
	PlayerTwoScore=0;
	//RemainingTime=20;
}

void ANetProGameState::AddScoreToPlayer(int32 PlayerID, int32 Score)
{
	if(!HasAuthority())
	{
		Server_AddScoreToPlayer(PlayerID,Score);
		return;
	}
	/*if(PlayerScoresMap.Contains(PlayerID))
	{
		PlayerScoresMap[PlayerID]+=Score;
	}else
	{//首次为玩家创建分数
		PlayerScoresMap.Add(PlayerID,Score);
		UE_LOG(LogTemp, Log, TEXT("首次为玩家%d创建分数"),PlayerID);
	}
	UE_LOG(LogTemp, Log, TEXT("服务器确认：玩家%d得分+%d,总分：%d"),
		PlayerID,Score,PlayerScoresMap[PlayerID]);*/
	if(PlayerID==1)
	{
		PlayerOneScore+=Score;
		OnScoreChanged.Broadcast(1,PlayerOneScore);//在服务器端Broadcast给服务器的UI更新
		UE_LOG(LogTemp, Log, TEXT("玩家%d得分+%d,总分%d"),PlayerID,Score,PlayerOneScore);

	}else if(PlayerID==2)
	{
		PlayerTwoScore+=Score;
		OnScoreChanged.Broadcast(2,PlayerTwoScore);//随着时间变量变化 服务器的UI更新
		UE_LOG(LogTemp, Log, TEXT("玩家%d得分+%d,总分%d"),PlayerID,Score,PlayerTwoScore);

	}
}


int32 ANetProGameState::GetPlayerScore(int32 PlayerID) const
{
	/*if(const int32* ScorePtr=PlayerScoresMap.Find(PlayerID))
	{
		return *ScorePtr;
	}*/
	if(PlayerID==1) return PlayerOneScore;
	if(PlayerID==2) return PlayerTwoScore;
	return 0;
}



void ANetProGameState::CheckWinner()
{
	if(!HasAuthority()){return;}
	int32 Winner=PlayerOneScore>PlayerTwoScore?1:PlayerOneScore<PlayerTwoScore?2:0;
	Multi_ShowWinnerBroadcast(Winner);

	//服务器通过MultiRPC 禁用输入
	Multi_DisablePlayerInput();
}
void ANetProGameState::Multi_ShowWinnerBroadcast_Implementation(int32 WinnerPlayerID)
{
	OnGameResult.Broadcast(WinnerPlayerID);
}



void ANetProGameState::StartGameCountDown(int32 Seconds)
{
	if(!HasAuthority()){return;}
	RemainingTime=Seconds;
	OnTimeChanged.Broadcast(RemainingTime);//服务器立即更新

	GetWorld()->GetTimerManager().SetTimer(CountDownTimer,[this]()
	{
		RemainingTime--;
		OnTimeChanged.Broadcast(RemainingTime);//循环显示 CountDown时间
		
		if(RemainingTime<=0)
		{
			GetWorld()->GetTimerManager().ClearTimer(CountDownTimer);
			CheckWinner();
		}
	},1.f,true);
}

/*void ANetProGameState::PurchaseItem(int32 ItemID)
{
	if(!PurchasedItemsArray.Contains(ItemID))
	{
		//UE_LOG(LogTemp, Log, TEXT("ANetProGameState::PurchaseItem(int32 ItemID)"));
		PurchasedItemsArray.Add(ItemID);
		//UE_LOG(LogTemp, Log, TEXT("添加购买的商品:%d"),ItemID);

		//ForceNetUpdate();
	}
}*/

/*void ANetProGameState::PurchaseItem(int32 ItemID)
{
	UE_LOG(LogTemp, Log, TEXT("GameState调用，HasAuthority:%s"),HasAuthority()?TEXT("true"):TEXT("false"));
	if(!HasAuthority())
	{
		Server_PurchaseItem(ItemID);
		return;
	}
	if(!PurchasedItemsArray.Contains(ItemID))
	{
		UE_LOG(LogTemp, Log, TEXT("ANetProGameState::PurchaseItem(int32 ItemID)"));
		PurchasedItemsArray.Add(ItemID);
		UE_LOG(LogTemp, Log, TEXT("添加购买的商品:%d"),ItemID);

		ForceNetUpdate();
	}
}*/
void ANetProGameState::PurchaseItem(int32 ItemID)
{
	//UE_LOG(LogTemp, Log, TEXT("GameState调用，HasAuthority:%s"),HasAuthority()?TEXT("true"):TEXT("false"));
	if(!HasAuthority())
	{
		UE_LOG(LogTemp, Log, TEXT("PurchaseItem方法只能在服务器上调用"));
		return;
	}
	if(!PurchasedItemsArray.Contains(ItemID))
	{
		PurchasedItemsArray.Add(ItemID);
		//UE_LOG(LogTemp, Log, TEXT("添加购买的商品:%d"),ItemID);

		//服务器上购买成功后 广播给所有客户端
		Multi_OnItemPurchased(ItemID);

		ForceNetUpdate();
	}
}

/*void ANetProGameState::OnRep_PurchasedItems()
{
	OnPurchasedItemChanged.Broadcast(PurchasedItemsArray);
}*/

/*void ANetProGameState::Server_PurchaseItem_Implementation(int32 ItemID)
{
	PurchaseItem(ItemID);
}

bool ANetProGameState::Server_PurchaseItem_Validate(int32 ItemID)
{
	return true;
}*/

bool ANetProGameState::IsItemPurchased(int32 ItemID) const
{
	return PurchasedItemsArray.Contains(ItemID);
}


void ANetProGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ANetProGameState,PlayerOneScore);
	DOREPLIFETIME(ANetProGameState,PlayerTwoScore);
	DOREPLIFETIME(ANetProGameState,RemainingTime);
	DOREPLIFETIME(ANetProGameState,PurchasedItemsArray);
}

void ANetProGameState::Server_AddScoreToPlayer_Implementation(int32 PlayerID, int32 Score)
{
	AddScoreToPlayer(PlayerID,Score);
}

bool ANetProGameState::Server_AddScoreToPlayer_Validate(int32 PlayerID, int32 Score)
{
	return true;
}


/*void ANetProGameState::Server_PurchaseItem_Implementation(int32 ItemID)
{
	PurchaseItem(ItemID);
}

bool ANetProGameState::Server_PurchaseItem_Validate(int32 ItemID)
{
	return true;
}*/


void ANetProGameState::Multi_OnItemPurchased_Implementation(int32 ItemID)
{
	OnPurchasedItemChanged.Broadcast(ItemID);
}

void ANetProGameState::OnRep_PlayerOneScore()
{
	OnScoreChanged.Broadcast(1,PlayerOneScore);//这是在客户端执行，更新客户端的UI
													//哪怕我是在服务器端操作导致的服务器玩家的分数PlayerOneScore变化
}


void ANetProGameState::OnRep_PlayerTwoScore()
{
	OnScoreChanged.Broadcast(2,PlayerTwoScore);
}

void ANetProGameState::OnRep_RemainingTime()
{
	OnTimeChanged.Broadcast(RemainingTime);
	/*if(RemainingTime<=0)
	{
		CheckWinner();
	}//因为只有服务器检查胜利者 广播结果Multi_ShowWinnerBroadcast_Implementation(int32 WinnerPlayerID)给各个终端
	写了也不走，void ANetProGameState::CheckWinner()
	{
		if(!HasAuthority()){return;}
		*/
}

void ANetProGameState::Multi_DisablePlayerInput_Implementation()
{
	/*UGameplayStatics::GetPlayerController(GetWorld(),0)->GetPawn()->DisableInput(
		UGameplayStatics::GetPlayerController(GetWorld(),0));
	UGameplayStatics::GetPlayerController(GetWorld(),1)->GetPawn()->DisableInput(
		UGameplayStatics::GetPlayerController(GetWorld(),1));*/
	for(auto It=GetWorld()->GetPlayerControllerIterator();It;++It)
	{
		if(APlayerController* PC=It->Get())
		{
			if(APawn* PlayerPawn=PC->GetPawn())
			{
				PlayerPawn->DisableInput(PC);
				//UE_LOG(LogTemp, Log, TEXT("禁用玩家输入"));
			}
		}
	}
}

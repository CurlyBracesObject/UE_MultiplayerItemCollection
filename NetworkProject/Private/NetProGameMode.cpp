// Fill out your copyright notice in the Description page of Project Settings.


#include "NetProGameMode.h"

#include "NetProGameState.h"
#include "NetProHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Player/NetProPlayerCharacter.h"
#include "Player/NetProPlayerController.h"
#include "Player/NetProPlayerStart.h"

ANetProGameMode::ANetProGameMode()
{
	ConstructorHelpers::FClassFinder<ANetProPlayerCharacter> PlayerCharacterClass(TEXT("Blueprint'/Game/NetProject/BPs/BP_PlayerCharacter.BP_PlayerCharacter_C'"));
	DefaultPawnClass=PlayerCharacterClass.Class;

	PlayerControllerClass=ANetProPlayerController::StaticClass();
	HUDClass=ANetProHUD::StaticClass();

	GameStateClass=ANetProGameState::StaticClass();
}

void ANetProGameMode::PostLogin(APlayerController* NewPlayer)//PostLogin延迟：解决PlayerID分配问题（Pawn创建时机）
{
	Super::PostLogin(NewPlayer);
	//获取当前的玩家数量 分配ID
	int32 PlayerCount=GetNumPlayers();
	int32 AssignedPlayerID=PlayerCount;

	//UE_LOG(LogTemp, Log, TEXT("当前玩家总数：%d"),PlayerCount);
	//UE_LOG(LogTemp, Log, TEXT("即将分配玩家ID：%d"),AssignedPlayerID);

	if(ANetProPlayerCharacter* PlayerChar=Cast<ANetProPlayerCharacter>(NewPlayer->GetPawn()))
	{
		PlayerChar->PlayerID=AssignedPlayerID;
		UE_LOG(LogTemp, Log, TEXT("分配玩家ID：%d"),AssignedPlayerID);
	}else
	{
		UE_LOG(LogTemp, Log, TEXT("找不到无法分配"));

		GetWorld()->GetTimerManager().SetTimerForNextTick([this,NewPlayer,AssignedPlayerID]()
		{
			if(ANetProPlayerCharacter* PlayerChar=Cast<ANetProPlayerCharacter>(NewPlayer->GetPawn()))
			{
				PlayerChar->PlayerID=AssignedPlayerID;
				UE_LOG(LogTemp, Log, TEXT("延迟分配AssignedPlayerID:%d"),AssignedPlayerID);
			}
		}

		);
	}
}

AActor* ANetProGameMode::ChoosePlayerStart_Implementation(AController* Player)//ChoosePlayerStart：解决生成位置问题
{
	int32 PlayerCount=GetNumPlayers();
	//UE_LOG(LogTemp, Log, TEXT("选择出生点，玩家数量=%d"),PlayerCount);

	//查找所有PlayerStarts
	TArray<AActor*> PlayerStarts;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),ANetProPlayerStart::StaticClass(),PlayerStarts);
	//UE_LOG(LogTemp, Log, TEXT("找到%d个NetProPlayerStart"),PlayerStarts.Num());

	for(AActor* Start:PlayerStarts)
	{
		if(ANetProPlayerStart* CustomStart=Cast<ANetProPlayerStart>(Start))
		{
			if(CustomStart->AssignedPlayerID==PlayerCount+1)
			{
				//UE_LOG(LogTemp, Log, TEXT("CustomStart->AssignedPlayerID是PlayerStart的ID=%d,PlayerCount是内置的方法表示的第%d个玩家连接"),
					//CustomStart->AssignedPlayerID,PlayerCount+1);
				return CustomStart;
			}
		}
	}
	UE_LOG(LogTemp, Log, TEXT("没找到匹配的PlayerStart"));
	return Super::ChoosePlayerStart_Implementation(Player);
}

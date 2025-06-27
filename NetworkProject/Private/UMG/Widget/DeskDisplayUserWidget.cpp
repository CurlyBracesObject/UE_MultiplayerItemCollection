// Fill out your copyright notice in the Description page of Project Settings.


#include "UMG/Widget/DeskDisplayUserWidget.h"

#include "NetProGameState.h"
#include "Components/Button.h"
#include "Player/NetProPlayerController.h"


UPackageComponent* UDeskDisplayUserWidget::GetPlayerPackageComponent()
{
	if(APlayerController* PC=GetOwningPlayer())
	{
		if(ANetProPlayerCharacter* PlayerCharacter=Cast<ANetProPlayerCharacter>(PC->GetPawn()))
		{
			return PlayerCharacter->GetPackageComponent();
		}
	}
	return nullptr;
}

void UDeskDisplayUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//绑定按钮点击事件
	if(PlaceItemsButton && !PlaceItemsButton->OnClicked.IsBound())
	{
		PlaceItemsButton->OnClicked.AddDynamic(this,&UDeskDisplayUserWidget::UDeskDisplayUserWidget::OnPlaceItemsButtonClicked);
	}

	//延迟绑定事件
	FTimerHandle BindTimer;
	GetWorld()->GetTimerManager().SetTimer(BindTimer,[this]()
	{
		if(ANetProGameState* GameState=GetWorld()->GetGameState<ANetProGameState>())
        	{
				if(!GameState->OnScoreChanged.IsBoundToObject(this))
				{
				//当GameState中的分数发生变化时，自动调用OnScoreChanged函数
        		GameState->OnScoreChanged.AddUObject(this,&UDeskDisplayUserWidget::OnScoreChanged);//第二次：响应分数变化（事件触发时）
               // UE_LOG(LogTemp, Log, TEXT("DeskDisplayUI绑定事件成功"));
					
					UpdateScoreDisplay();//第一次：立即更新（初始化时）
				//UE_LOG(LogTemp, Log, TEXT("首先更新一次"));
				}
        	}
	},0.1f,false);
	
}

void UDeskDisplayUserWidget::UpdateScoreDisplay()
{
	if(ScoreText)
	{
		if(ANetProPlayerController* PC=Cast<ANetProPlayerController>(GetOwningPlayer()))
		{
			if(ANetProPlayerCharacter* PlayerChar=Cast<ANetProPlayerCharacter>(PC->GetPawn()))
			{
				if(ANetProGameState* GameState=GetWorld()->GetGameState<ANetProGameState>())
				{
					//int32 PlayerScore=GameState->GetPlayerScore(PlayerChar->PlayerID);
					//ScoreText->SetText(FText::FromString(FString::Printf(TEXT("玩家%d分数：%d"),PlayerChar->PlayerID,PlayerScore)));
					int32 Player1Score=GameState->GetPlayerScore(1);
					int32 Player2Score=GameState->GetPlayerScore(2);
					ScoreText->SetText(FText::FromString(FString::Printf(TEXT("玩家1分数是%d,玩家2分数是%d"),
						Player1Score,Player2Score)));

				}
			}
		}
	}
}

void UDeskDisplayUserWidget::OnPlaceItemsButtonClicked()
{
	UPackageComponent* PackageComp=GetPlayerPackageComponent();
	if(PackageComp)
	{
		/*TMap<int32,int32> Package=PackageComp->GetPackage();

		for(const auto& Items:Package)
		{
			UE_LOG(LogTemp, Log, TEXT(" UDeskDisplayUserWidget::OnPlaceItemsButtonClicked()里的for"));
			int32 Position=Items.Key;
			int32 ItemID=Items.Value;
			PackageComp->RemoveItemFromPackage(Position);
			PackageComp->PlaceItemsOnDesk(ItemID,DeskLocation);
		}*/
		PackageComp->PlaceItemsOnDeskAndClearUI(DeskLocation);
	}
}

void UDeskDisplayUserWidget::OnScoreChanged(int32 PlayerID,int32 Score)
{
	UpdateScoreDisplay();
}

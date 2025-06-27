// Fill out your copyright notice in the Description page of Project Settings.


#include "UMG/PackageUserWidget.h"

#include "Blueprint/DragDropOperation.h"
#include "UMG/Widget/ItemsInScrollBoxWidget.h"

#include "Player/NetProPlayerCharacter.h"
#include "UMG/Widget/SingleItemInPackageWidget.h"

void UPackageUserWidget::AddToScreen(ULocalPlayer* LocalPlayer, int32 ZOrder)
{
	Super::AddToScreen(LocalPlayer, ZOrder);
	if(ANetProPlayerCharacter* PlayerCharacter=Cast<ANetProPlayerCharacter>(GetOwningPlayer()->GetPawn()))
	{
		for(auto Item:PlayerCharacter->GetPackageComponent()->GetNearSceneItemActors())
		{
			NearbyItemsInScrollBoxWidget->AddSingleItemWidgetToScrollBox(Item);
		}
		//UPackageComponent的NotifyPackageItem广播的监听   
		PlayerCharacter->GetPackageComponent()->OnPackageItemAdded.AddUObject(this,&UPackageUserWidget::OnPackageAddItem);
													//拿广播对象     AddUObject注册监听函数
		PlayerCharacter->GetPackageComponent()->OnPackageItemRemoved.AddUObject(this,&UPackageUserWidget::OnPackageRemoveItem);
	}
}

void UPackageUserWidget::RemoveFromParent()//ANetProHUD::TogglePackage()调用的 每次开每次调用
									//删除SingleItemInPackageWidget的是 玩家离开Overlap直接ScrollBox里的ClearScrollBox()方法
{
	Super::RemoveFromParent();
	if(NearbyItemsInScrollBoxWidget)
	{
		NearbyItemsInScrollBoxWidget->ClearScrollBox();
	}
	if(GetOwningPlayer())
	{
		if(ANetProPlayerCharacter* PlayerCharacter=Cast<ANetProPlayerCharacter>(GetOwningPlayer()->GetPawn()))
		{
			PlayerCharacter->GetPackageComponent()->OnPackageItemAdded.RemoveAll(this);//每次开 消除物品信息广播出去的绑定
			PlayerCharacter->GetPackageComponent()->OnPackageItemRemoved.RemoveAll(this);
		}
	}
}

bool UPackageUserWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	if(USingleItemInPackageWidget* SingleItemInPackageWidget=Cast<USingleItemInPackageWidget>(InOperation->Payload))
	{
		if(!SingleItemInPackageWidget->GetSceneItemActorPassBySingleItemUI())//这个物品已经经历过放入背包的过程 场景里没有这个物品
		{
			if(ANetProPlayerCharacter* PlayerCharacter=Cast<ANetProPlayerCharacter>(GetOwningPlayerPawn()))
			{
				if(PlayerCharacter->GetPackageComponent())
				{
					//UE_LOG(LogTemp, Log, TEXT("SingleItemInPackageWidget->GetSingleItemUIPosition():%05d"),SingleItemInPackageWidget->GetSingleItemUIPosition());
					PlayerCharacter->GetPackageComponent()->RemoveItemFromPackage(SingleItemInPackageWidget->GetSingleItemUIPosition());
					PlayerCharacter->GetPackageComponent()->SpawnNearbySceneItem(SingleItemInPackageWidget->GetSingleItemUIPosition());
					return true;
				}
			}
		}
	}
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}

void UPackageUserWidget::OnPackageAddItem(int32 Position, int32 ID)
{
	PackageItemsInScrollBoxWidget->AddSingleItemWidgetToScrollBox(Position,ID);
}

void UPackageUserWidget::OnPackageRemoveItem(int32 Position, int32 ID)
{
	PackageItemsInScrollBoxWidget->RemoveSingleItemUI(Position);
	//UE_LOG(LogTemp, Log, TEXT("void UPackageUserWidget::OnPackageRemoveItem(int32 Position, int32 ID)"));
}

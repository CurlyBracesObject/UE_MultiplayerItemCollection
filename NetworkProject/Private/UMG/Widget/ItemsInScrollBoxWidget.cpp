// Fill out your copyright notice in the Description page of Project Settings.


#include "UMG/Widget/ItemsInScrollBoxWidget.h"

#include "Blueprint/DragDropOperation.h"
#include "Components/ScrollBox.h"
#include "Player/NetProPlayerCharacter.h"
#include "UMG/Widget/SingleItemInPackageWidget.h"

#include "Scene/SceneItemActor.h"

void UItemsInScrollBoxWidget::AddSingleItemWidgetToScrollBox(ASceneItemActor* ItemActor)
								//ANetProPlayerCharacter::OnComponentBeginOverlap里调用的
{
	TSubclassOf<USingleItemInPackageWidget> SingleItemUIInScrollClass=LoadClass<USingleItemInPackageWidget>(nullptr,TEXT("WidgetBlueprint'/Game/NetProject/UMG/Widget/Widget_SingleItemInPackage.Widget_SingleItemInPackage_C'"));
	USingleItemInPackageWidget* SingleItemInPackageUI=CreateWidget<USingleItemInPackageWidget>(GetOwningPlayer(),SingleItemUIInScrollClass);
	if(SingleItemInPackageUI)
	{
		SingleItemInPackageUI->InitSingleItemUIPanel(ItemActor);
		MyScrollBox->AddChild(SingleItemInPackageUI);
	}
}

void UItemsInScrollBoxWidget::AddSingleItemWidgetToScrollBox(int32 Position, int32 ID)
{
	TSubclassOf<USingleItemInPackageWidget> SingleItemUIInScrollClass=LoadClass<USingleItemInPackageWidget>(nullptr,TEXT("WidgetBlueprint'/Game/NetProject/UMG/Widget/Widget_SingleItemInPackage.Widget_SingleItemInPackage_C'"));
	USingleItemInPackageWidget* SingleItemInPackageUI=CreateWidget<USingleItemInPackageWidget>(GetOwningPlayer(),SingleItemUIInScrollClass);
	if(SingleItemInPackageUI)
	{
		SingleItemInPackageUI->InitSingleItemUIPanel(Position,ID);
		MyScrollBox->AddChild(SingleItemInPackageUI);
	}
}

void UItemsInScrollBoxWidget::RemoveSingleItemUI(ASceneItemActor* ItemActor)
		//void UPackageUserWidget::OnPackageRemoveItem(int32 Position, int32 ID)调用的这个方法
			//PackageComponent里面来广播    UPackageUserWidget监听    void UPackageUserWidget::OnPackageRemoveItem(int32 Position, int32 ID)监听
{
	for(int32 i=0;i<MyScrollBox->GetChildrenCount();i++)
	{
		if(USingleItemInPackageWidget* SingleItemInPackageWidget=Cast<USingleItemInPackageWidget>(MyScrollBox->GetChildAt(i)))
		{
			if(SingleItemInPackageWidget->GetSceneItemActorPassBySingleItemUI()==ItemActor)
			{
				SingleItemInPackageWidget->RemoveFromParent();
				break;
			}
		}
	}
}

void UItemsInScrollBoxWidget::RemoveSingleItemUI(int32 Position)
{
	for(int32 i=0;i<MyScrollBox->GetChildrenCount();i++)
	{
		if(USingleItemInPackageWidget* SingleItemInPackageWidget=Cast<USingleItemInPackageWidget>(MyScrollBox->GetChildAt(i)))
		{
			if(SingleItemInPackageWidget->GetSingleItemUIPosition()==Position)
			{
				SingleItemInPackageWidget->RemoveFromParent();
				//UE_LOG(LogTemp, Log, TEXT("UItemsInScrollBoxWidget::RemoveSingleItemUI(int32 Position)SingleItemInPackageWidget->RemoveFromParent()"));
				break;
			}
		}
	}
}

void UItemsInScrollBoxWidget::ClearScrollBox()
{
	MyScrollBox->ClearChildren();
}


bool UItemsInScrollBoxWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
                                           UDragDropOperation* InOperation)
{
	if(!bPackageScrollBox)
	{
		return false;
	}
	if(USingleItemInPackageWidget* SingleItemWidget=Cast<USingleItemInPackageWidget>(InOperation->Payload))
	{
		//MyScrollBox->AddChild(SingleItemWidget);
		if(ANetProPlayerCharacter* PlayerCharacter=Cast<ANetProPlayerCharacter>(GetOwningPlayerPawn()))
		{
			if(PlayerCharacter->GetPackageComponent())
			{
				//ASceneItemActor* SceneItem=SingleItemWidget->GetSceneItemActorPassBySingleItemUI();
				SingleItemWidget->RemoveFromParent();
				//UE_LOG(LogTemp, Log, TEXT("SingleItemWidget已经从NearByScrollBox移除UI"));
				PlayerCharacter->GetPackageComponent()->AddItemToPackage(SingleItemWidget->GetSceneItemActorPassBySingleItemUI());
				//UE_LOG(LogTemp, Log, TEXT("SingleItemWidget已经加到PacakgeScrollBox里"));
				//这个是在给数据层面的 背包物品 
			}
		}
	}
	
	return true;
}



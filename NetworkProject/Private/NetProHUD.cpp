// Fill out your copyright notice in the Description page of Project Settings.


#include "NetProHUD.h"

#include "NetProGameState.h"
#include "Components/CanvasPanelSlot.h"
#include "Engine/Canvas.h"

#include "UMG/PackageUserWidget.h"
#include "UMG/Widget/DeskDisplayUserWidget.h"
#include "Player/NetProPlayerCharacter.h"

#include "Desk/DeskActor.h"

#include "UMG/MainGameUserWidget.h"


void ANetProHUD::TogglePackage()
{
	if(!PackageUI)
	{
		TSubclassOf<UPackageUserWidget> PackageClass=LoadClass<UPackageUserWidget>(nullptr,TEXT("WidgetBlueprint'/Game/NetProject/UMG/UMG_Package.UMG_Package_C'"));
		PackageUI=CreateWidget<UPackageUserWidget>(GetOwningPlayerController(),PackageClass);
	}
	if(PackageUI && !PackageUI->IsInViewport())
	{
		PackageUI->AddToViewport();
	}else
		{
			PackageUI->RemoveFromParent();
		}
	
}

void ANetProHUD::ShowDeskDisplayUI(ADeskActor* DeskActor,FVector DeskWorldLocation)
{
	if(!DeskDisplayUI)
	{
		TSubclassOf<UDeskDisplayUserWidget> DeskDisplayClass=LoadClass<UDeskDisplayUserWidget>(nullptr,TEXT("WidgetBlueprint'/Game/NetProject/UMG/Widget/Widget_DeskDisplay.Widget_DeskDisplay_C'"));
		DeskDisplayUI=CreateWidget<UDeskDisplayUserWidget>(GetOwningPlayerController(),DeskDisplayClass);
	}
	if(DeskDisplayUI && !DeskDisplayUI->IsInViewport())
	{
		DeskDisplayUI->AddToViewport();

		//改变 显示玩家得分 的text内容
		//DeskDisplayUI->UpdateScoreDisplay();

		//把桌子位置给DisplayDeskUI
		DeskDisplayUI->SetDeskLocation(DeskWorldLocation);

		/*//转换 物体 世界坐标 到 UI 屏幕坐标
		FVector2D DeskBottomScreenPosition;
		if(GetOwningPlayerController()->ProjectWorldLocationToScreen(DeskBottomWorldLocation,DeskBottomScreenPosition))
		{
			if(UCanvasPanelSlot* CanvasSlot=Cast<UCanvasPanelSlot>(DeskDisplayUI->Slot))
				//把UI添加到屏幕时，UE会自动创建一个"槽位"(Slot)
				//不管你的UI内部结构是什么，AddToViewport() 总是创建一个隐形的Canvas Panel作为根容器。
				/*
				* // UE在背后做了：
// 1. 创建一个隐形的Canvas Panel覆盖整个屏幕
// 2. 把你的DeskDisplayUI放进这个Canvas Panel
// 3. 为DeskDisplayUI创建一个CanvasPanelSlot来管理它的位置
// 4. DeskDisplayUI->Slot 就是这个CanvasPanelSlot对象
				 #1#
			{
				CanvasSlot->SetPosition(DeskBottomScreenPosition);
			}
		}*/
		// 计算桌子下方的世界坐标（向下偏移一些距离）
		//FVector DeskBottomWorldLocation = DeskWorldLocation + FVector(0, 0, -150); // 向下偏移150单位
   
		// 转换世界坐标到屏幕坐标
		FVector2D DeskScreenPosition;
		if(GetOwningPlayerController()->ProjectWorldLocationToScreen(DeskWorldLocation, DeskScreenPosition))
		{
			if(UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(DeskDisplayUI->Slot))
			{
				// 设置UI位置，可以添加一些偏移让UI更好看
				FVector2D UIOffset = FVector2D(0, 50); // 向下偏移50像素
				CanvasSlot->SetPosition(DeskScreenPosition + UIOffset);
         
				// 设置UI锚点为中心，这样UI会以中心点为基准定位
				CanvasSlot->SetAnchors(FAnchors(0.0f, 0.0f, 0.0f, 0.0f));
				CanvasSlot->SetAlignment(FVector2D(0.5f, 0.5f)); // 中心对齐
			}
		}
	}
}

void ANetProHUD::HideDeskDispalyUI()
{
	if(DeskDisplayUI)
	{
		DeskDisplayUI->RemoveFromParent();
	}
}

void ANetProHUD::BeginPlay()
{
	Super::BeginPlay();

	if(!MainGameUI)
	{
		TSubclassOf<UMainGameUserWidget> MainGameClass=LoadClass<UMainGameUserWidget>(nullptr,TEXT("WidgetBlueprint'/Game/NetProject/UMG/UMG_MainGame.UMG_MainGame_C'"));
		MainGameUI=CreateWidget<UMainGameUserWidget>(GetOwningPlayerController(),MainGameClass);
	}
	if(MainGameUI)
	{
		MainGameUI->AddToViewport();
		
	}
	//注意要HUD调用这个里面的方法才会走逻辑
	if(GetOwningPlayerController()->HasAuthority())
	{
		if(ANetProGameState* GameState=GetWorld()->GetGameState<ANetProGameState>())
		{
			GameState->StartGameCountDown(20);
		}
	}
}




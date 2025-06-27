// Fill out your copyright notice in the Description page of Project Settings.


#include "UMG/Widget/SingleItemInPackageWidget.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Subsystem/ItemOnTheGroundSubsystem.h"

void USingleItemInPackageWidget::InitSingleItemUIPanel(ASceneItemActor* SceneItemActor)
{
	if(!SceneItemActor)
	{
		return;
	}
	const FSingleItemBase* SingleItemBase=GetWorld()->GetGameInstance()->GetSubsystem<UItemOnTheGroundSubsystem>()->GetSingleItemByID(SceneItemActor->GetID());

	if(SingleItemBase)
	{
		NameTextBlock->SetText(SingleItemBase->Name);
		IconImage->SetBrushFromTexture(SingleItemBase->Icon);
	}
	SceneItemActorPassBySingleItemUI=SceneItemActor;
}

void USingleItemInPackageWidget::InitSingleItemUIPanel(int32 Position, int32 ID)
{
	SingleItemUIPosition=Position;
	const FSingleItemBase* SingleItemBase=GetWorld()->GetGameInstance()->GetSubsystem<UItemOnTheGroundSubsystem>()->GetSingleItemByID(ID);

	if(SingleItemBase)
	{
		NameTextBlock->SetText(SingleItemBase->Name);
		IconImage->SetBrushFromTexture(SingleItemBase->Icon);
	}
}

FReply USingleItemInPackageWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry,
                                                           const FPointerEvent& InMouseEvent)
{
	return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent,this,EKeys::LeftMouseButton).NativeReply;
}

void USingleItemInPackageWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	UDragDropOperation*& OutOperation)
{
	OutOperation=UWidgetBlueprintLibrary::CreateDragDropOperation(UDragDropOperation::StaticClass());
	OutOperation->DefaultDragVisual=this;
	OutOperation->Payload=this;
	OutOperation->Pivot=EDragPivot::MouseDown;
}

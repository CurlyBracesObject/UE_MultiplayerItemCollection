// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Scene/SceneItemActor.h"
#include "SingleItemInPackageWidget.generated.h"


class UTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class NETWORKPROJECT_API USingleItemInPackageWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void InitSingleItemUIPanel(ASceneItemActor* SceneItemActor);
	void InitSingleItemUIPanel(int32 Position,int32 ID);

	ASceneItemActor* GetSceneItemActorPassBySingleItemUI(){return SceneItemActorPassBySingleItemUI;}

	int32 GetSingleItemUIPosition(){return SingleItemUIPosition;}
	
protected:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

protected:
	UPROPERTY(meta=(BindWidget))
	UTextBlock* NameTextBlock;
	UPROPERTY(meta=(BindWidget))
	UImage* IconImage;

	ASceneItemActor* SceneItemActorPassBySingleItemUI;

	int32 SingleItemUIPosition;
};

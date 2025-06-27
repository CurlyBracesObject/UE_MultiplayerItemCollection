// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemsInScrollBoxWidget.generated.h"

class UScrollBox;
class ASceneItemActor;
/**
 * 
 */
UCLASS()
class NETWORKPROJECT_API UItemsInScrollBoxWidget : public UUserWidget
{
	GENERATED_BODY()


protected:
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

public:
	void AddSingleItemWidgetToScrollBox(ASceneItemActor* ItemActor);//AddPackageBoard(ASceneItemActor* ItemActor)
	void AddSingleItemWidgetToScrollBox(int32 Position,int32 ID);

	void RemoveSingleItemUI(ASceneItemActor* ItemActor);
	void RemoveSingleItemUI(int32 Position);

	
	void ClearScrollBox();
	
protected:
	UPROPERTY(meta=(BindWidget))
	UScrollBox* MyScrollBox;
	UPROPERTY(EditAnywhere)
	uint8 bPackageScrollBox:1;
};

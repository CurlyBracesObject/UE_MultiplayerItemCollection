// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PackageUserWidget.generated.h"

class UItemsInScrollBoxWidget;
/**
 * 
 */
UCLASS()
class NETWORKPROJECT_API UPackageUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void AddToScreen(ULocalPlayer* LocalPlayer, int32 ZOrder) override;

	virtual void RemoveFromParent() override;

	UItemsInScrollBoxWidget* GetNearbyItemsInScrollBoxWidget(){return NearbyItemsInScrollBoxWidget;}

protected:
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	void OnPackageAddItem(int32 Position,int32 ID);
	void OnPackageRemoveItem(int32 Position,int32 ID);

protected:
	UPROPERTY(meta=(BindWidget))
	UItemsInScrollBoxWidget* NearbyItemsInScrollBoxWidget;
	UPROPERTY(meta=(BindWidget))
	UItemsInScrollBoxWidget* PackageItemsInScrollBoxWidget;
};

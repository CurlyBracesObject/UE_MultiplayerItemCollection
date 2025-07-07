// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget/ShopInfoUserWidget.h"
#include "ShopUserWidget.generated.h"

class UShopItemsScrollBoxWidget;
/**
 * 
 */
UCLASS()
class NETWORKPROJECT_API UShopUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	UShopInfoUserWidget* ShopInfoUI;
	UPROPERTY(meta=(BindWidget))
	UShopItemsScrollBoxWidget* ShopItemsInScrollBoxWidget;
};

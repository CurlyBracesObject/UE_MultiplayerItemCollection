// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ShopItemSubsystem.generated.h"


USTRUCT()
struct FShopSingleItemBase
{
	GENERATED_BODY()
	int32 ID;
	FText Name;
	UTexture2D* Icon;
	int32 Price;
};

UENUM()
enum class EShopItemSkinType:uint8
{
	EST_None,
	EST_Hair,
	EST_Glasses,
	EST_Helmet,
	EST_FaceMask,
	EST_Bag,
	EST_EyeMask,
	EST_Coat,
	EST_Beard
};


USTRUCT()
struct FShopItemSkin:public FShopSingleItemBase
{
	GENERATED_BODY()

	FShopItemSkin():StaticMesh(nullptr),SkeletalMesh(nullptr),SkinType(EShopItemSkinType::EST_None){}
	UStaticMesh* StaticMesh;
	USkeletalMesh* SkeletalMesh;
	EShopItemSkinType SkinType;
};


/**
 * 
 */
UCLASS()
class NETWORKPROJECT_API UShopItemSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	
public:
	
	const FShopSingleItemBase* GetSingleItemByID(int32 ID);

	//获取所有道具以供商店使用
	//UFUNCTION(BlueprintCallable)
	TArray<FShopItemSkin*> GetAllSkins();
	void LoadItemsFromJson();

	

protected:
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;
	
	//资源路径解析和加载
	UTexture2D* LoadIconFromPath(const FString& IconPath);
	
	EShopItemSkinType StringToSkinType(const FString& TypeString);

protected:

	TMap<int32,FShopItemSkin*> SkinMap;
	
};

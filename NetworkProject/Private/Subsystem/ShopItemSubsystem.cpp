// Fill out your copyright notice in the Description page of Project Settings.



#include "Subsystem/ShopItemSubsystem.h"
#include "Subsystem/ShopItemSubsystem.h"
#include "Engine/Texture2D.h"
#include "Engine/StaticMesh.h"
#include "Engine/SkeletalMesh.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "HAL/PlatformFilemanager.h"

const FShopSingleItemBase* UShopItemSubsystem::GetSingleItemByID(int32 ID)
{
	if(	SkinMap.Contains(ID))
	{
		return SkinMap[ID];
	}
	return nullptr;
}

TArray<FShopItemSkin*> UShopItemSubsystem::GetAllSkins()
{
	TArray<FShopItemSkin*> AllSkins;

	for(auto& Pair:SkinMap)
	{
		AllSkins.Add(Pair.Value);
	}
	return AllSkins;
}

void UShopItemSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	LoadItemsFromJson();
	UE_LOG(LogTemp, Log, TEXT("道具系统初始化完成，加载了%d个道具"),SkinMap.Num());
}

void UShopItemSubsystem::Deinitialize()
{
	for(auto& Pair:SkinMap)
	{
		if(Pair.Value)
		{
			delete Pair.Value;
		}
	}
	SkinMap.Empty();
	Super::Deinitialize();
}

void UShopItemSubsystem::LoadItemsFromJson()
{
	FString JsonPath=FPaths::ProjectContentDir()+TEXT("NetProject/Data/ItemData.json");
	FString JsonString;
	const TCHAR* PathPtr=*JsonPath;

	if(!FFileHelper::LoadFileToString(JsonString,PathPtr))//文件变成JsonString ItemData.json → JsonString
	{
		UE_LOG(LogTemp, Log, TEXT("找不到Json文件%s"),*JsonPath);
		return;
	}
	UE_LOG(LogTemp, Log, TEXT("Json内容 %s"),*JsonString);
	TSharedRef<TJsonReader<TCHAR>> JsonReader=TJsonReaderFactory<TCHAR>::Create(JsonString);//字符串→读取器
	TArray<TSharedPtr<FJsonValue>> JsonArray;

	if(FJsonSerializer::Deserialize(JsonReader,JsonArray))//读取器JsonReader变成JsonArray内存对象 加载进内存
	{
		UE_LOG(LogTemp, Log, TEXT("Json解析成功，道具数量：%d"),JsonArray.Num());
		for (auto& Item:JsonArray)
		{
			TSharedPtr<FJsonObject> ItemObject=Item->AsObject();
			if(ItemObject.IsValid())
			{
				//关键：创建FShopItemSkin指针
				FShopItemSkin* NewSkin=new FShopItemSkin();

				//商店里展示的信息
				NewSkin->ID=ItemObject->GetIntegerField(TEXT("ID"));
				NewSkin->Name=FText::FromString(ItemObject->GetStringField(TEXT("Name")));
				
				FString IconPath=ItemObject->GetStringField(TEXT("Icon"));
				NewSkin->Icon=LoadIconFromPath(IconPath);
				
				NewSkin->Price=ItemObject->GetIntegerField(TEXT("Price"));

				//真正的模型
				FString StaticMeshPath=ItemObject->GetStringField(TEXT("StaticMesh"));
				if(!StaticMeshPath.IsEmpty())
				{
					NewSkin->StaticMesh=LoadObject<UStaticMesh>(nullptr,*StaticMeshPath);
				}
				
				FString SkeletalMeshPath=ItemObject->GetStringField(TEXT("SkeletalMesh"));
				if(!SkeletalMeshPath.IsEmpty())
				{
					NewSkin->SkeletalMesh=LoadObject<USkeletalMesh>(nullptr,*SkeletalMeshPath);
				}
				
				FString SkinTypeStr=ItemObject->GetStringField(TEXT("SkinType"));
				NewSkin->SkinType=StringToSkinType(SkinTypeStr);
				

				SkinMap.Add(NewSkin->ID,NewSkin);
				UE_LOG(LogTemp, Log, TEXT("ID=%d,Name=%s,Price=%d,Icon=%s"),
					NewSkin->ID,*NewSkin->Name.ToString(),NewSkin->Price,
					NewSkin->Icon?TEXT("加载成功"):TEXT("加载失败"));
			}
		}
	}else
	{
		UE_LOG(LogTemp, Log, TEXT("Json解析失败"));
	}
}


UTexture2D* UShopItemSubsystem::LoadIconFromPath(const FString& IconPath)
{
	if(IconPath.IsEmpty())
	{
		UE_LOG(LogTemp, Log, TEXT("Icon路径为空"));
		return nullptr;
	}
	UE_LOG(LogTemp, Log, TEXT("加载IconPath:%s"),*IconPath);
	UTexture2D* LoadedIcon=LoadObject<UTexture2D>(nullptr,*IconPath);
	if(!LoadedIcon)
	{
		UE_LOG(LogTemp, Log, TEXT("无法加载:%s"),*IconPath);
	}
	return LoadedIcon;
}

EShopItemSkinType UShopItemSubsystem::StringToSkinType(const FString& TypeString)
{
	if(TypeString==TEXT("EST_Hair"))return EShopItemSkinType::EST_Hair;
	if(TypeString==TEXT("EST_Glasses"))return EShopItemSkinType::EST_Glasses;
	if(TypeString==TEXT("EST_Helmet"))return EShopItemSkinType::EST_Helmet;
	if(TypeString==TEXT("EST_FaceMask"))return EShopItemSkinType::EST_FaceMask;
	if(TypeString==TEXT("EST_Bag"))return EShopItemSkinType::EST_Bag;
	if(TypeString==TEXT("EST_EyeMask"))return EShopItemSkinType::EST_EyeMask;
	if(TypeString==TEXT("EST_Coat"))return EShopItemSkinType::EST_Coat;
	if(TypeString==TEXT("EST_Beard"))return EShopItemSkinType::EST_Beard;

	return EShopItemSkinType::EST_None;
}


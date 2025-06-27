// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/ItemOnTheGroundSubsystem.h"

const FSingleItemBase* UItemOnTheGroundSubsystem::GetSingleItemByID(int32 ID)
{
	if(SkinMap.Contains(ID))
	{
		return SkinMap[ID];
	}
	return nullptr;
}

void UItemOnTheGroundSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	SkinDataTable=LoadObject<UDataTable>(this,TEXT("DataTable'/Game/NetProject/Data/Props/DT_Skin.DT_Skin'"));
	const TMap<FName,uint8*> Data=SkinDataTable->GetRowMap();

	for(auto Item:Data)
	{
		int ID=0;
		FString Left,Right;
		if(Item.Key.ToString().Split(TEXT("_"),&Left,&Right))
		{

			ID=FCString::Atoi(*Right)+1;
		}

		if(!SkinMap.Contains(ID))
		{
			SkinMap.Add(ID,reinterpret_cast<FSkin*>(Item.Value));
			//没有关系的指针类型转换 reinterpret_cast<转换成的FSkin*>(uint8*)   Data map的Item.Value是uint8*
		}
	}
}

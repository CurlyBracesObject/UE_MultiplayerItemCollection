// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ItemOnTheGroundSubsystem.generated.h"

UENUM()
enum class ESingleItemType:uint8
{
	EIT_SKin,
	EIT_Weapon
};



USTRUCT()
struct FSingleItemBase:public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	int32 ID;
	UPROPERTY(EditAnywhere)
	FText Name;
	UPROPERTY(EditAnywhere)
	UTexture2D* Icon;
	
	ESingleItemType Type;
};









UENUM()
enum class ESkinType:uint8
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



UENUM()
enum class EGender:uint8
{
	EG_Male,
	EG_Female
};



USTRUCT()
struct FSkin:public FSingleItemBase
{
	GENERATED_BODY()

	FSkin(): StaticMesh(nullptr), SkeletalMesh(nullptr), SkinType(ESkinType::EST_None), Gender(EGender::EG_Female)
	{
		Type = ESingleItemType::EIT_SKin;
	}
	
	UPROPERTY(EditAnywhere)
	UStaticMesh* StaticMesh;
	UPROPERTY(EditAnywhere)
	USkeletalMesh* SkeletalMesh;
	UPROPERTY(EditAnywhere)
	ESkinType SkinType;
	UPROPERTY(EditAnywhere)
	EGender Gender;
};



/**
 * 
 */
UCLASS()
class NETWORKPROJECT_API UItemOnTheGroundSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	const FSingleItemBase* GetSingleItemByID(int32 ID);
protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
protected:
	UPROPERTY()
	UDataTable* SkinDataTable;

	TMap<int32,FSkin*> SkinMap;
};

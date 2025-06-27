// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PackageComponent.generated.h"


class ASceneItemActor;

DECLARE_MULTICAST_DELEGATE_TwoParams(NotifyPackageItem,int32,int32);
//UPackageUserWidget::OnPackageAddItem是它的监听函数 让主背包UI、ItemsInScrollBoxWidget监听
//（void UPackageUserWidget::OnPackageRemoveItem(int32 Position, int32 ID)）
	//继续调用PackageItemsInScrollBoxWidget->AddSingleItemWidgetToScrollBox(Position,ID);
	//里面又继续调用USingleItemInPackageWidget里的InitSingleItemUIPanel(int32 Position,int32 ID);



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NETWORKPROJECT_API UPackageComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPackageComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	int32 GetSingleItemUIPositionInPackage();

	int32 GetOwnerPlayerID() const;

	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void AddNearSceneItemActor(ASceneItemActor* NearSceneItemActor);
	void RemoveNearSceneItemActor(ASceneItemActor* NearSceneItemActor);

	//向背包中添加道具
	void AddItemToPackage(ASceneItemActor* SceneItemActor);
	void AddItemToPackage(int32 ID);
	
	void RemoveItemFromPackage(int32 Position);
	
	void SpawnNearbySceneItem(int32 Position);
	
	TArray<ASceneItemActor*> GetNearSceneItemActors();

	void PlaceItemsOnDeskAndClearUI(FVector DeskLocation);
	void PlaceItemsOnDesk(int32 ItemID,FVector DeskLocation);

	TMap<int32,int32> GetPackage() const{return Package;}

protected:
	UFUNCTION(Server,Reliable,WithValidation)
	void Server_AddItemToPackage(ASceneItemActor* SceneItemActor);
	UFUNCTION(Server,Reliable,WithValidation)
	void Server_PlaceItemsOnDesk(int32 ItemID, FVector DeskLocation);

protected:
	UPROPERTY()
	TArray<ASceneItemActor*> NearSceneItemActorArray;

	TMap<int32,int32> Package;//key:装的道具在背包中的位置信息  value:SceneItemActor ID

public:
	//背包道具被添加
	NotifyPackageItem OnPackageItemAdded;//找Broadcast 把Item信息给Package 
	//背包道具被移除
	NotifyPackageItem OnPackageItemRemoved;
	

};

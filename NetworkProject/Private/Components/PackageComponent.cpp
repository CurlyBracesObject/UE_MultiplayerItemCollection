// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PackageComponent.h"

#include "NetProGameState.h"
#include "NetProHUD.h"
#include "Character/NetProCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "Player/NetProPlayerCharacter.h"
#include "Player/NetProPlayerController.h"
#include "Scene/SceneItemActor.h"
#include "UMG/Widget/DeskDisplayUserWidget.h"

// Sets default values for this component's properties
UPackageComponent::UPackageComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicated(true);//开启同步 具有广播能力属于终端玩家 有权力向服务器上消息

	// ...
}


// Called when the game starts
void UPackageComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

int32 UPackageComponent::GetSingleItemUIPositionInPackage()
{
	int32 Position=0;
	while (Package.Contains(Position))
	{
		Position++;
	}
	return Position;
}

int32 UPackageComponent::GetOwnerPlayerID() const
{
	if(AActor* Owner=GetOwner())
	{
		if(ANetProPlayerCharacter* PlayerChar=Cast<ANetProPlayerCharacter>(Owner))
		{
			return PlayerChar->PlayerID;
		}
	}
	return 0;
}


// Called every frame
void UPackageComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPackageComponent::AddNearSceneItemActor(ASceneItemActor* NearSceneItemActor)
{
	NearSceneItemActorArray.Add(NearSceneItemActor);
}

void UPackageComponent::RemoveNearSceneItemActor(ASceneItemActor* NearSceneItemActor)
{
	NearSceneItemActorArray.Remove(NearSceneItemActor);
}

void UPackageComponent::AddItemToPackage(ASceneItemActor* SceneItemActor)
{
	
	/*UE_LOG(LogTemp, Log, TEXT("[%s]开始拾取%s"),GetOwner()->HasAuthority()?TEXT("server"):TEXT("client"),
		SceneItemActor?*SceneItemActor->GetName():TEXT("NULL"));*/
	if(!SceneItemActor)
	{
		UE_LOG(LogTemp, Log, TEXT("对象为空"));
		return;
	}
	if(!IsValid(SceneItemActor))
	{
		UE_LOG(LogTemp, Log, TEXT("对象无效，可能已经被标记为销毁"));
		return;
	}
	if(SceneItemActor->IsPendingKill())
	{
		UE_LOG(LogTemp, Log, TEXT("SceneItemActor正在被销毁"));
		return;
	}
	AddItemToPackage(SceneItemActor->GetID());
	/*if(!GetOwner()->HasAuthority())
	{
		UE_LOG(LogTemp, Log, TEXT("客户端开始走ServerRPC"));
		Server_AddItemToPackage(SceneItemActor);
	}*/
	if(GetWorld()->GetNetMode()==NM_Client)
	{
		//UE_LOG(LogTemp, Log, TEXT("客户端开始走ServerRPC"));
		Server_AddItemToPackage(SceneItemActor);
		return;
	}
	if(IsValid(SceneItemActor))//对象有效再销毁
	{
		SceneItemActor->Destroy();//销毁场景中的道具
	}
	
	
}

void UPackageComponent::AddItemToPackage(int32 ID)
{
	int32 Position=GetSingleItemUIPositionInPackage();
	Package.Add(Position,ID);
	if(OnPackageItemAdded.IsBound())
	{//NotifyPackageItem是这个的广播类型 目的是把物体的数据传给背包主UI→SrollBox→SingleItemUI
		OnPackageItemAdded.Broadcast(Position,ID);
	}
}

void UPackageComponent::RemoveItemFromPackage(int32 Position)
{
	if(Package.Contains(Position))
	{
		//NotifyPackageItem是这个的广播类型 目的是把物体的数据移除背包
		if(OnPackageItemRemoved.IsBound())
		{
			OnPackageItemRemoved.Broadcast(Position,Package[Position]);
		}
		UE_LOG(LogTemp, Log, TEXT("UPackageComponent::RemoveItemFromPackage(int32 Position)里的广播"));

		
		//UE_LOG(LogTemp, Log, TEXT("RemoveItemFromPackage(int32 Position)的Spawn:%05d"),Position);
		//SpawnNearbySceneItem(Position);//NativeDrop的InOperation->Payload把Position数据一路传过来
				//我在背包主UI的Drop里直接加了PlayerCharacter->GetPackageComponent()->SpawnNearbySceneItem(SingleItemInPackageWidget->GetSingleItemUIPosition());
		Package.Remove(Position);

	}
}

void UPackageComponent::SpawnNearbySceneItem(int32 Position)
{
	if(ANetProCharacterBase* CharacterBase=Cast<ANetProCharacterBase>(GetOwner()))
	{
		//FTransform Tran;
		FVector CharacterLocation=CharacterBase->GetActorLocation();
		float CapsuleHalfHeight=CharacterBase->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		CharacterLocation.Z-=CapsuleHalfHeight;//角色脚底位置
		FVector GroundLocation=CharacterLocation;
		
		
		FVector SpawnLocation=GroundLocation+
			FRotator(0,FMath::RandRange(0.f,360.f),0).Vector()*FMath::RandRange(50,150);

		//抬高一点避免贴地
		//SpawnLocation.Z+=5.f;
		
		ASceneItemActor* SceneItemActor=CharacterBase->GetWorld()->SpawnActorDeferred<ASceneItemActor>(ASceneItemActor::StaticClass(),FTransform(SpawnLocation));
		SceneItemActor->SetID(Package[Position]);
		SceneItemActor->FinishSpawning(FTransform(SpawnLocation));
	}
}


TArray<ASceneItemActor*> UPackageComponent::GetNearSceneItemActors()
{
	return NearSceneItemActorArray;
}

void UPackageComponent::PlaceItemsOnDeskAndClearUI(FVector DeskLocation)
{
	TArray<TPair<int32,int32>> ItemsToPlace;

	//收集所有物品
	for(const auto& Item:Package)
	{
		ItemsToPlace.Add(TPair<int32,int32>(Item.Key,Item.Value));
	}

	//逐个处理
	for(const auto& Item:ItemsToPlace)
	{
		int32 Position=Item.Key;
		int32 ItemID=Item.Value;

		if(OnPackageItemRemoved.IsBound())
		{
			OnPackageItemRemoved.Broadcast(Position,ItemID);
		}
		PlaceItemsOnDesk(ItemID,DeskLocation);
	}
	
	Package.Empty();
}

void UPackageComponent::PlaceItemsOnDesk(int32 ItemID, FVector DeskLocation)
{
	if(!GetOwner()->HasAuthority())
	{
		Server_PlaceItemsOnDesk(ItemID,DeskLocation);
		return;
	}
	FVector SpawnLocation=DeskLocation+FVector(
		FMath::RandRange(-60.f,60.f),
		FMath::RandRange(-60.f,60.f),
		88.f
	);

	if(UWorld* World=GetWorld())
	{
		ASceneItemActor* SceneItemActor=World->SpawnActorDeferred<ASceneItemActor>(ASceneItemActor::StaticClass(),FTransform(SpawnLocation));
		if(SceneItemActor)
		{
			SceneItemActor->SetID(ItemID);
			SceneItemActor->FinishSpawning(FTransform(SpawnLocation));

			int32 PlayerID=GetOwnerPlayerID();
			if(ANetProGameState* GameState=World->GetGameState<ANetProGameState>())
			{
				GameState->AddScoreToPlayer(PlayerID,1);
				UE_LOG(LogTemp, Log, TEXT("玩家%d放置物品 ID：%d,获得1分"),PlayerID,ItemID);

				/*//这个是放一个更新一次得分 我并不愿意这样 我想全部放才更新得分UI
				if(ANetProPlayerCharacter* PlayerChar=Cast<ANetProPlayerCharacter>(GetOwner()))
				{
					if(ANetProPlayerController* PC=PlayerChar->GetController<ANetProPlayerController>())
					{
						if(ANetProHUD* NetProHUD=Cast<ANetProHUD>(PC->GetHUD()))
						{
							NetProHUD->DeskDisplayUI->UpdateScoreDisplay();
						}
					}
				}*/
				
			}
			
			//UE_LOG(LogTemp, Log, TEXT("在桌子上生成物品，ID：%d,位置：%s"),ItemID,*SpawnLocation.ToString());
		}
	}
}


void UPackageComponent::Server_AddItemToPackage_Implementation(ASceneItemActor* SceneItemActor)
{
	if(SceneItemActor)
	{
		AddItemToPackage(SceneItemActor);//在客户端带一个指针到服务器 服务器怎么能找到对应的指针对象
												//actor或组件开启了网络同步  或 摆放在场景中的物品有可靠性命名 摆放在场景运行时候能加载进来
	}
}

bool UPackageComponent::Server_AddItemToPackage_Validate(ASceneItemActor* SceneItemActor)
{
	return true;
}

void UPackageComponent::Server_PlaceItemsOnDesk_Implementation(int32 ItemID, FVector DeskLocation)
{
	PlaceItemsOnDesk(ItemID,DeskLocation);
}

bool UPackageComponent::Server_PlaceItemsOnDesk_Validate(int32 ItemID, FVector DeskLocation)
{
	return true;
}
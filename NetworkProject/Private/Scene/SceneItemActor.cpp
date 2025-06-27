// Fill out your copyright notice in the Description page of Project Settings.


#include "Scene/SceneItemActor.h"

#include "Components/BillboardComponent.h"
#include "Net/UnrealNetwork.h"
#include "NetworkProject/NetworkProject.h"
#include "Subsystem/ItemOnTheGroundSubsystem.h"

// Sets default values
ASceneItemActor::ASceneItemActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	RootComponent=CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	BillboardComponent=CreateDefaultSubobject<UBillboardComponent>(TEXT("BillboardComp"));
	BillboardComponent->SetupAttachment(RootComponent);
	bReplicates=true;

}

// Called when the game starts or when spawned
void ASceneItemActor::BeginPlay()
{
	Super::BeginPlay();
	InitActor();
	TArray<UPrimitiveComponent*> Components;
	GetComponents<UPrimitiveComponent>(Components);
	for(UPrimitiveComponent* Comp:Components)
	{
		Comp->SetGenerateOverlapEvents(true);
		Comp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		//设置collision的物体类别通道
		Comp->SetCollisionObjectType(ECC_SceneItem);
		
	}
	
}

void ASceneItemActor::InitActor()
{
		const FSingleItemBase* SingleItemBase=GetWorld()->GetGameInstance()->GetSubsystem<UItemOnTheGroundSubsystem>()->GetSingleItemByID(ID);
		if(SingleItemBase)
		{
			if(SingleItemBase->Type==ESingleItemType::EIT_SKin)
			{
				const FSkin* Skin=static_cast<const FSkin*>(SingleItemBase);
				//有关系的指针类型转换 static_cast<const FSkin*转换成的子类 结构体指针>(SingleItemBase父类 结构体指针)
				if(Skin->StaticMesh)
				{
					GetStaticMeshComponent()->SetStaticMesh(Skin->StaticMesh);
					//UE的UStaticMeshComponent内置里有个方法SetStaticMesh（UStaticMesh* StaticMesh）
					//GetStaticMeshComponent()返回UStaticMeshComponent*
					//Skin->StaticMesh返回UStaticMesh*
				}else if(Skin->SkeletalMesh)
				{
					GetSkeletalMeshComponent()->SetSkeletalMesh(Skin->SkeletalMesh);
				}
				
			}
		}
}

UStaticMeshComponent* ASceneItemActor::GetStaticMeshComponent()
{
	if(StaticMeshComponent)
	{
		return StaticMeshComponent;
	}
	//动态创建组件
	StaticMeshComponent=NewObject<UStaticMeshComponent>(this);
	StaticMeshComponent->RegisterComponentWithWorld(GetWorld());
	StaticMeshComponent->AttachToComponent(RootComponent,FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	return StaticMeshComponent;
}

USkeletalMeshComponent* ASceneItemActor::GetSkeletalMeshComponent()
{
	if(SkeletalMeshComponent)
	{
		return SkeletalMeshComponent;
	}
	SkeletalMeshComponent=NewObject<USkeletalMeshComponent>(this);
	SkeletalMeshComponent->RegisterComponentWithWorld(GetWorld());
	SkeletalMeshComponent->AttachToComponent(RootComponent,FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	return SkeletalMeshComponent;
}

// Called every frame
void ASceneItemActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASceneItemActor::OnRep_ID()
{
	InitActor();//ID同步后立即更新模型
}

void ASceneItemActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASceneItemActor,ID);
}


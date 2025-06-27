// Fill out your copyright notice in the Description page of Project Settings.


#include "Desk/DeskActor.h"

#include "NetProHUD.h"
#include "Components/BoxComponent.h"
#include "NetworkProject/NetworkProject.h"
#include "Player/NetProPlayerController.h"

// Sets default values
ADeskActor::ADeskActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent=CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));

	StaticMeshComponent=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticComp"));
	StaticMeshComponent->SetupAttachment(RootComponent);

	BoxComponent=CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComponent->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ADeskActor::BeginPlay()
{
	Super::BeginPlay();
	BoxComponent->SetGenerateOverlapEvents(true);
	BoxComponent->SetCollisionProfileName(TEXT("OverlapAll"));

	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComponent->SetCollisionObjectType(ECC_Desk);
	//BoxComponent->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);

	BoxComponent->OnComponentBeginOverlap.RemoveDynamic(this,&ADeskActor::OnComponentBeginOverlap);
	BoxComponent->OnComponentEndOverlap.RemoveDynamic(this,&ADeskActor::OnComponentEndOverlap);
	
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this,&ADeskActor::OnComponentBeginOverlap);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this,&ADeskActor::OnComponentEndOverlap);
	
}

void ADeskActor::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Log, TEXT("ADeskActor::OnComponentBeginOverlap"));
	if(ANetProPlayerController* PC=Cast<ANetProPlayerController>(OtherActor->GetInstigatorController()))
	{
		if(PC->IsLocalController())
		{
			if(ANetProHUD* HUD=Cast<ANetProHUD>(PC->GetHUD()))
			{
				//传递桌子的世界位置
				//FVector DeskBottomLocation=GetActorLocation()+FVector(0,0,-100);
				FVector DeskLocation=GetActorLocation();
				HUD->ShowDeskDisplayUI(this,DeskLocation);
			}
		}
		
	}
}

void ADeskActor::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Log, TEXT("ADeskActor::OnComponentEndOverlap"));
	if(ANetProPlayerController* PC=Cast<ANetProPlayerController>(OtherActor->GetInstigatorController()))
	{
		if(PC->IsLocalController())
		{
			if(ANetProHUD* HUD=Cast<ANetProHUD>(PC->GetHUD()))
			{
				HUD->HideDeskDispalyUI();
			}
		}
		
	}
}

// Called every frame
void ADeskActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


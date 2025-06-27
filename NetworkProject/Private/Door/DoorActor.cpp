// Fill out your copyright notice in the Description page of Project Settings.


#include "Door/DoorActor.h"

#include "Character/NetProCharacterBase.h"

// Sets default values
ADoorActor::ADoorActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent=CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	
	StaticMeshComponent=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticComp"));
	StaticMeshComponent->SetupAttachment(RootComponent);

	BoxComponent=CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComponent->SetupAttachment(RootComponent);
	/*玩家总是在门前的固定区域触发开门，不会因为门的状态而改变
触发区域代表"门的影响范围"，而不是"门本身的一部分"*/
}

// Called when the game starts or when spawned
void ADoorActor::BeginPlay()
{
	Super::BeginPlay();
	BoxComponent->SetGenerateOverlapEvents(true);
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this,&ADoorActor::OnComponentBeginOverlap);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this,&ADoorActor::OnComponentEndOverlap);
}

void ADoorActor::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(Cast<ANetProCharacterBase>(OtherActor))
	{
		//UE_LOG(LogTemp, Log, TEXT("beginOverlap"));
		bPlayAnim=true;
		CurveDirection=1;
	}
	OpenDir=FVector::DotProduct(GetActorRightVector(),(OtherActor->GetActorLocation()-GetActorLocation()).GetSafeNormal())<0?1:-1;
}

void ADoorActor::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(Cast<ANetProCharacterBase>(OtherActor))
	{
		//UE_LOG(LogTemp, Log, TEXT("EndOverlap"));
		bPlayAnim=true;
		CurveDirection=-1;
	}
}

void ADoorActor::UpdateAnim(float DeltaTime)
{
	if(!bPlayAnim || !DoorAnim)
	{
		return;
	}

	CurrentCurveTime+=(DeltaTime*CurveDirection);

	float Min,Max;
	DoorAnim->GetValueRange(Min,Max);
	if(CurrentCurveTime<Min)
	{
		bPlayAnim=false;
		CurrentCurveTime=Min;
	}
	if(CurrentCurveTime>Max)
	{
		bPlayAnim=false;
		CurrentCurveTime=Max;
	}

	float Value=DoorAnim->GetFloatValue(CurrentCurveTime);
	StaticMeshComponent->SetRelativeRotation(FRotator(0,90*Value*OpenDir,0));
}

// Called every frame
void ADoorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateAnim(DeltaTime);
}


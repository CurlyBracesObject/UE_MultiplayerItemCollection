// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NetProCharacterBase.h"

#include "Components/NetProCharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ANetProCharacterBase::ANetProCharacterBase(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<UNetProCharacterMovementComponent>(CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PackageComponent=CreateDefaultSubobject<UPackageComponent>(TEXT("PackageComp"));
}

// Called when the game starts or when spawned
void ANetProCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ANetProCharacterBase::StartSprint()
{
	bSprint=true;//玩家按键 → bSprint=true → 立即看到冲刺效果
				/*客户端预测的核心思想
					"先行动，再同步"：*/
	if(!HasAuthority())
	{
		Server_ChangeSprint(true);
	}
}

void ANetProCharacterBase::StopSprint()
{
	bSprint=false;
	if(!HasAuthority())
	{
		Server_ChangeSprint(false);
	}
}

void ANetProCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ANetProCharacterBase,bSprint);
}

void ANetProCharacterBase::Server_ChangeSprint_Implementation(bool bOutSprint)
{
	if(bOutSprint)
	{
		StartSprint();
	}else
	{
		StopSprint();
	}
}

bool ANetProCharacterBase::Server_ChangeSprint_Validate(bool bOutSprint)
{
	return true;
}

// Called every frame
void ANetProCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ANetProCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

bool ANetProCharacterBase::IsSprinting()
{
	return bSprint&&FVector::DotProduct(GetVelocity().GetSafeNormal(),GetActorForwardVector())>0.9f;
	//按了Shift 并且向w a s d方向上移动了
}


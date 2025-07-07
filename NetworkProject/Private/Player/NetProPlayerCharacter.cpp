// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/NetProPlayerCharacter.h"

#include "NetProHUD.h"
#include "Actors/ShopActor.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Net/UnrealNetwork.h"
#include "NetworkProject/NetworkProject.h"
#include "Player/NetProPlayerController.h"
#include "Scene/SceneItemActor.h"
#include "Subsystem/ShopItemSubsystem.h"
#include "UMG/Widget/ItemsInScrollBoxWidget.h"

#include "UMG/PackageUserWidget.h"
#include "UMG/ShopUserWidget.h"
#include "UMG/MainGameUserWidget.h"
#include "UMG/Widget/ShopItemsScrollBoxWidget.h"

ANetProPlayerCharacter::ANetProPlayerCharacter(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	SpringArmComponent=CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	CameraComponent=CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));

	SpringArmComponent->SetupAttachment(RootComponent);
	CameraComponent->SetupAttachment(SpringArmComponent);

	SpringArmComponent->bUsePawnControlRotation=true;//能跟随控制器旋转

	SphereComponent=CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComponent->SetupAttachment(RootComponent);
	SphereComponent->SetSphereRadius(150.f);
	
	if(UCapsuleComponent* CapsuleComp=GetCapsuleComponent())
	{
		CapsuleComp->SetCollisionObjectType(ECC_Pawn);
	}
}

void ANetProPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	if(SphereComponent)
	{
		SphereComponent->SetGenerateOverlapEvents(true);
		SphereComponent->SetCollisionProfileName(TEXT("PlayerSphere"));
		SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

		SphereComponent->SetCollisionResponseToChannel(ECC_SceneItem,ECR_Overlap);
		SphereComponent->SetCollisionResponseToChannel(ECC_WorldDynamic,ECR_Overlap);
		SphereComponent->SetCollisionResponseToChannel(ECC_Desk,ECR_Overlap);
		
		SphereComponent->OnComponentBeginOverlap.AddDynamic(this,&ANetProPlayerCharacter::OnComponentBeginOverlap);
		SphereComponent->OnComponentEndOverlap.AddDynamic(this,&ANetProPlayerCharacter::OnComponentEndOverlap);
	}

	/*FTimerHandle IDCheckTimer;
	GetWorld()->GetTimerManager().SetTimer(IDCheckTimer,[this]()
	{
		FString NetMode;
		if(HasAuthority())
		{
			NetMode=TEXT("服务器端");
		}else if(IsLocallyControlled())
		{
			NetMode=TEXT("本地控制");
		}else
		{
			NetMode=TEXT("其他玩家");
		}
		UE_LOG(LogTemp, Warning, TEXT("%s玩家角色确认，我的PlayerID是:%d"),*NetMode,PlayerID);
	},1.f,false);*/
	
}

void ANetProPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"),this,&ANetProPlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"),this,&ANetProPlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("Turn"),this,&ANetProPlayerCharacter::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"),this,&ANetProPlayerCharacter::LookUp);
	PlayerInputComponent->BindAction(TEXT("DoJump"),IE_Pressed,this,&ANetProPlayerCharacter::DoJump);
	PlayerInputComponent->BindAction(TEXT("DoSprint"),IE_Pressed,this,&ANetProPlayerCharacter::StartSprint);
	PlayerInputComponent->BindAction(TEXT("DoSprint"),IE_Released,this,&ANetProPlayerCharacter::StopSprint);
	PlayerInputComponent->BindAction(TEXT("TogglePackage"),IE_Pressed,this,&ANetProPlayerCharacter::TogglePackageUI);
}

void ANetProPlayerCharacter::MoveForward(float Value)
{
	if(Value==0)return;
	AddMovementInput(FRotator(0,GetControlRotation().Yaw,0).Quaternion().GetAxisX(),Value);
}

void ANetProPlayerCharacter::MoveRight(float Value)
{
	if(Value==0)return;
	AddMovementInput(FRotator(0,GetControlRotation().Yaw,0).Quaternion().GetAxisY(),Value);
}

void ANetProPlayerCharacter::DoJump()
{
	Jump();
}

void ANetProPlayerCharacter::Turn(float Value)
{
	AddControllerYawInput(Value*GetWorld()->GetDeltaSeconds()*35);
}

void ANetProPlayerCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value*GetWorld()->GetDeltaSeconds()*35);
}

void ANetProPlayerCharacter::TogglePackageUI()
{
	if(ANetProPlayerController* PC=Cast<ANetProPlayerController>(GetController()))
	{
		if(ANetProHUD* HUD=Cast<ANetProHUD>(PC->GetHUD()))
		{
			HUD->TogglePackage();
		}
	}
}

void ANetProPlayerCharacter::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(ASceneItemActor* SceneItemActor=Cast<ASceneItemActor>(OtherActor))
	{
		//UE_LOG(LogTemp, Log, TEXT("Begin"));
		if(PackageComponent)
		{
			PackageComponent->AddNearSceneItemActor(SceneItemActor);
		}
		if(ANetProPlayerController* PC=Cast<ANetProPlayerController>(GetController()))
		{
			if(ANetProHUD* HUD=Cast<ANetProHUD>(PC->GetHUD()))
			{
				if(UPackageUserWidget* PackageUserWidget=HUD->GetPackageUI())
				{
					PackageUserWidget->GetNearbyItemsInScrollBoxWidget()->AddSingleItemWidgetToScrollBox(SceneItemActor);
				}
			}
		}
	}

	if(AShopActor* ShopActor=Cast<AShopActor>(OtherActor))
	{
		if(ANetProPlayerController* PC=Cast<ANetProPlayerController>(GetController()))
		{
			if(PC->IsLocalController())
			{
				if(ANetProHUD* MyHUD=Cast<ANetProHUD>(PC->GetHUD()))
				{
					//获取商店子系统
					UShopItemSubsystem* ShopSubsystem=GetGameInstance()->GetSubsystem<UShopItemSubsystem>();
					if(ShopSubsystem)
					{
						TArray<FShopItemSkin*> AllSkins=ShopSubsystem->GetAllSkins();

						UShopUserWidget* MainShopUI=MyHUD->MainGameUI->GetShopMainWidget();

						if(MainShopUI&&MainShopUI->ShopInfoUI)
						{
							MainShopUI->ShopInfoUI->InitShopInfoUI();
						}
					
						if(MainShopUI&&MainShopUI->ShopItemsInScrollBoxWidget)
						{
							MainShopUI->ShopItemsInScrollBoxWidget->ClearScrollBox();

							//使用json中的真实ID
							for(FShopItemSkin* Skin:AllSkins)
							{
								if(Skin)
								{
									MainShopUI->ShopItemsInScrollBoxWidget->AddSingleItemInShopWidgetToScrollBox(Skin->ID);
								}
							}
						}
					}
					MyHUD->MainGameUI->GetShopMainWidget()->SetVisibility(ESlateVisibility::Visible);

					/*//添加输入设置
					PC->SetInputMode(FInputModeGameAndUI());
					PC->SetShowMouseCursor(true);*/
				}
			}
		}
	}
	
}

void ANetProPlayerCharacter::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(ASceneItemActor* SceneItemActor=Cast<ASceneItemActor>(OtherActor))
	{
		//UE_LOG(LogTemp, Log, TEXT("End"));
		if(PackageComponent)
		{
			PackageComponent->RemoveNearSceneItemActor(SceneItemActor);
		}
		if(ANetProPlayerController* PC=Cast<ANetProPlayerController>(GetController()))
		{
			if(ANetProHUD* HUD=Cast<ANetProHUD>(PC->GetHUD()))
			{
				if(UPackageUserWidget* PackageUserWidget=HUD->GetPackageUI())
				{
					PackageUserWidget->GetNearbyItemsInScrollBoxWidget()->ClearScrollBox();
				}
			}
		}
	}
	if(ANetProPlayerController* PC=Cast<ANetProPlayerController>(GetController()))
	{
		if(PC->IsLocalController())
		{
			if(ANetProHUD* MyHUD=Cast<ANetProHUD>(PC->GetHUD()))
			{
				MyHUD->MainGameUI->GetShopMainWidget()->SetVisibility(ESlateVisibility::Hidden);
				
			}
		}
	}
}

void ANetProPlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ANetProPlayerCharacter,PlayerID);
}

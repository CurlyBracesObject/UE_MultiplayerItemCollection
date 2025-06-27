// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "NetProHUD.generated.h"

class UPackageUserWidget;
class UDeskDisplayUserWidget;
class ANetProPlayerCharacter;
class ADeskActor;
class UMainGameUserWidget;
/**
 * 
 */
UCLASS()
class NETWORKPROJECT_API ANetProHUD : public AHUD
{
	GENERATED_BODY()


public:
	void TogglePackage();

	UPackageUserWidget* GetPackageUI(){return PackageUI;}

	void ShowDeskDisplayUI(ADeskActor* DeskActor,FVector DeskWorldLocation=FVector::ZeroVector);

	void HideDeskDispalyUI();

protected:
	virtual void BeginPlay() override;

	
public:
	UPROPERTY()
	UDeskDisplayUserWidget* DeskDisplayUI;
protected:
	UPROPERTY()
	UPackageUserWidget* PackageUI;

	UPROPERTY()
	UMainGameUserWidget* MainGameUI;

	/*ANetProPlayerCharacter* PlayerCharacter;
	ADeskActor* DeskActor;
	*/


};

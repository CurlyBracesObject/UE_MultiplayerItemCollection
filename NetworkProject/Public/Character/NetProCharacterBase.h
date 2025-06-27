// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PackageComponent.h"
#include "GameFramework/Character.h"
#include "NetProCharacterBase.generated.h"

UCLASS()
class NETWORKPROJECT_API ANetProCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANetProCharacterBase(const FObjectInitializer& ObjectInitializer);
	UPackageComponent* GetPackageComponent(){return PackageComponent;}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void StartSprint();
	void StopSprint();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//Net
	UFUNCTION(Server,Reliable,WithValidation)
	void Server_ChangeSprint(bool bOutSprint);
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool IsSprinting();

protected:
	UPROPERTY(Replicated)
	bool bSprint;

	UPROPERTY(VisibleAnywhere)
	UPackageComponent* PackageComponent;
};

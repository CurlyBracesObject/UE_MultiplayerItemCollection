// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SceneItemActor.generated.h"

UCLASS()
class NETWORKPROJECT_API ASceneItemActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASceneItemActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void InitActor();

	UStaticMeshComponent* GetStaticMeshComponent();
	USkeletalMeshComponent* GetSkeletalMeshComponent();
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	int32 GetID() const {return ID;}//表示这个成员函数不会修改对象的状态
	void SetID(int32 OutID){ID=OutID;}
	
protected:
	UFUNCTION()
	void OnRep_ID();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
protected:
	UPROPERTY(VisibleAnywhere)
	UBillboardComponent* BillboardComponent;
	UPROPERTY(ReplicatedUsing=OnRep_ID,EditAnywhere)
	int32 ID;//ReplicatedUsing 会在ID同步到客户端时自动调用 OnRep_ID()自动调用InitActor()，但是Replicated客户端收到ID但不会自动调用InitActor()

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMeshComponent;
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* SkeletalMeshComponent;
};

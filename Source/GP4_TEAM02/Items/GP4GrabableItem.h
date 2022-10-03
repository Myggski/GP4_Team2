// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GP4_TEAM02/Arms/GP4GrabableItemsInterface.h"
#include "GP4GrabableItem.generated.h"

UCLASS()
class GP4_TEAM02_API AGP4GrabableItem final : public AActor, public IGP4GrabableItemsInterface
{
	GENERATED_BODY()
public:
	AGP4GrabableItem();
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* StaticMeshComponent;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
private:
	const FName DEFAULT_HANDLE_NAME = "handle";
	UFUNCTION()
	void OnComponentSleep(UPrimitiveComponent* SleepingComponent, FName BoneName);
	void AddHighlight_Implementation() override;
	void RemoveHighlight_Implementation() override;
	FVector GetGrabOffsetLocation_Implementation(FName HandleSocketName) override;
	FRotator GetGrabRotation_Implementation(FName HandleSocketName) override;
};

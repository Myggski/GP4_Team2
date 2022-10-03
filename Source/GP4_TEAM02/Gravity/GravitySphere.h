// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "GravitySphere.generated.h"

class USphereComponent;

UCLASS()
class GP4_TEAM02_API AGravitySphere : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGravitySphere();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent* GravitySphere;

	UPROPERTY (EditAnywhere)
	bool DebugInsideGravitySphere = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float GravityStrength = 980;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<UPrimitiveComponent*> ComponentArray;
	
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};

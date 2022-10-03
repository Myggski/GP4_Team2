// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GP4ArmMovementConfigStruct.h"
#include "Components/ActorComponent.h"
#include "GP4ArmMovementComponent.generated.h"

class UGP4ArmForceCalculator;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GP4_TEAM02_API UGP4ArmMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGP4ArmMovementComponent();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void GetLeftHandMovement(FVector Input, FVector HandPosition, bool bIsInFocusMode, FVector &NewPosition);

	UFUNCTION(BlueprintCallable)
	void GetRightHandMovement(FVector Input, FVector HandPosition, bool bIsInFocusMode, FVector &NewPosition);

	UPROPERTY(BlueprintReadWrite)
	bool bIsUsingGamepad;
	
	float InputAxisLastFrame = 0.0f;
	
	UPROPERTY(BlueprintReadOnly)
	UGP4ArmForceCalculator* ArmForceCalculator;

	UPROPERTY(BlueprintReadWrite)
	USkeletalMeshComponent* ArmsMesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGP4ArmMovementConfigStruct MovementConfig;
};

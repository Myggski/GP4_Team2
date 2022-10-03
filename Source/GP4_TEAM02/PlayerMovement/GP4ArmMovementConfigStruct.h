// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GP4ArmMovementConfigStruct.generated.h"

USTRUCT(BlueprintType)
struct GP4_TEAM02_API FGP4ClampVector2DStruct
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	float Min;
	UPROPERTY(EditDefaultsOnly)
	float Max;

	FGP4ClampVector2DStruct()
	{
		Min = 0.0f;
		Max = 0.0f;
	}

	FGP4ClampVector2DStruct(float MinValue, float MaxValue)
	{
		Min = MinValue;
		Max = MaxValue;
	}
};


USTRUCT(BlueprintType)
struct GP4_TEAM02_API FGP4ArmMovementConfigStruct
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	float HorizontalSpeed;
	UPROPERTY(EditDefaultsOnly)
	float VerticalSpeed;
	UPROPERTY(EditDefaultsOnly)
	float HorizontalFocusSpeed;
	UPROPERTY(EditDefaultsOnly)
	float VerticalFocusSpeed;

	UPROPERTY(EditDefaultsOnly)
	FGP4ClampVector2DStruct LeftHandHorizontalClamp;
	UPROPERTY(EditDefaultsOnly)
	FGP4ClampVector2DStruct RightHandHorizontalClamp;
	UPROPERTY(EditDefaultsOnly)
	FGP4ClampVector2DStruct HandVerticalClamp;

	FGP4ArmMovementConfigStruct()
	{
		HorizontalSpeed = 1000.0f;
		VerticalSpeed = 1000.0f;
		HorizontalFocusSpeed = 800.0f;
		VerticalFocusSpeed = 800.0f;

		LeftHandHorizontalClamp = {-70.0f, 70.0f};
		RightHandHorizontalClamp = {-120.0f, 70.0f};
		HandVerticalClamp = {40.0f, 180.0f};
	}
};
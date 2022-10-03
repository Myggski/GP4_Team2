// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GP4ArmAnimationPositions.generated.h"

USTRUCT(BlueprintType)
struct GP4_TEAM02_API FGP4ArmAnimationPositions
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float RArmForward = 0.0f;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float RArmRight = 0.0f;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float RArmUp = 0.0f;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float LArmForward = 0.0f;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float LArmRight = 0.0f;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float LArmUp = 0.0f;

	FGP4ArmAnimationPositions()
	{
		RArmForward = 0.0f;
		RArmRight = 0.0f;
		RArmUp = 0.0f;

		LArmForward = 0.0f;
		LArmRight = 0.0f;
		LArmUp = 0.0f;
	}
};

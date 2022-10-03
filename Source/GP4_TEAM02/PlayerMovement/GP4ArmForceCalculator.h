#pragma once
#include "GP4ArmMovementConfigStruct.h"

#include "GP4ArmForceCalculator.generated.h"

UCLASS()
class GP4_TEAM02_API UGP4ArmForceCalculator : public UObject
{
	GENERATED_BODY()

public:
	UGP4ArmForceCalculator();

	void Init(FGP4ArmMovementConfigStruct* ConfigValues);
	
	float CalculateHorizontalMovement(float InputAxis, float CurrentAxisPos, bool bIsInFocusMode, bool RightArm) const;
	float CalculateVerticalMovement(float InputAxis, float CurrentAxisPos, bool bIsInFocusMode) const;
	bool SnapToViewFrustum(const FVector& WorldPos, FVector* OutSnappedPos);
	
private:

	float HorizontalSpeed;
	float VerticalSpeed;
	float HorizontalFocusSpeed;
	float VerticalFocusSpeed;
	
	TQueue<float> RightHandHorizontalInputAxisLastFrame;
	TQueue<float> LeftHandHorizontalInputAxisLastFrame;

	FGP4ClampVector2DStruct LeftHandHorizontalClamp;
	FGP4ClampVector2DStruct RightHandHorizontalClamp;
	FGP4ClampVector2DStruct HandVerticalClamp;

	UPROPERTY()
	APlayerCameraManager* CameraManager;
};
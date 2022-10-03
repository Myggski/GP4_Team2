
#include "GP4ArmForceCalculator.h"
#include "Kismet/GameplayStatics.h"

UGP4ArmForceCalculator::UGP4ArmForceCalculator()
{
	
}

void UGP4ArmForceCalculator::Init(FGP4ArmMovementConfigStruct* ConfigValues)
{
	HorizontalSpeed = ConfigValues->HorizontalSpeed;
	VerticalSpeed = ConfigValues->VerticalSpeed;
	HorizontalFocusSpeed = ConfigValues->HorizontalFocusSpeed;
	VerticalFocusSpeed = ConfigValues->VerticalFocusSpeed;

	LeftHandHorizontalClamp = ConfigValues->LeftHandHorizontalClamp;
	RightHandHorizontalClamp = ConfigValues->RightHandHorizontalClamp;
	HandVerticalClamp = ConfigValues->HandVerticalClamp;

	CameraManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
}

float UGP4ArmForceCalculator::CalculateHorizontalMovement(float InputAxis, float CurrentAxisPos, bool bIsInFocusMode, bool RightArm) const
{
	float HorizontalAcceleration = 0.0f;
	
	if (bIsInFocusMode)
	{
		HorizontalAcceleration = InputAxis * HorizontalFocusSpeed * GetWorld()->GetDeltaSeconds();
	}

	else
	{
		HorizontalAcceleration = InputAxis * HorizontalSpeed * GetWorld()->GetDeltaSeconds();
	}
	
	float NewAxisPos = HorizontalAcceleration + CurrentAxisPos;

	if (RightArm)
	{
		return FMath::Clamp(NewAxisPos, RightHandHorizontalClamp.Min, RightHandHorizontalClamp.Max);
	}

	return FMath::Clamp(NewAxisPos, LeftHandHorizontalClamp.Min, RightHandHorizontalClamp.Max);
}

float UGP4ArmForceCalculator::CalculateVerticalMovement(float InputAxis, float CurrentAxisPos, bool bIsInFocusMode) const
{
	float VerticalAcceleration = 0.0f;
	
	if (bIsInFocusMode)
	{
		VerticalAcceleration = InputAxis * VerticalFocusSpeed * GetWorld()->GetDeltaSeconds();
	}

	else
	{
		VerticalAcceleration = InputAxis * VerticalSpeed * GetWorld()->GetDeltaSeconds();
	}
	
	float NewAxisPos = VerticalAcceleration + CurrentAxisPos;

	return FMath::Clamp(NewAxisPos, HandVerticalClamp.Min, HandVerticalClamp.Max);
}

bool UGP4ArmForceCalculator::SnapToViewFrustum(const FVector& WorldPos, FVector* OutSnappedPos)
{
	ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(GetWorld()->GetFirstLocalPlayerFromController());
	if (LocalPlayer != NULL && LocalPlayer->ViewportClient != NULL && LocalPlayer->ViewportClient->Viewport != NULL)
	{
		// This sceneview initialization should probably be moved into another function.
		// Create a view family for the game viewport
		FSceneViewFamilyContext ViewFamily(FSceneViewFamily::ConstructionValues(
			LocalPlayer->ViewportClient->Viewport,
			GetWorld()->Scene,
			LocalPlayer->ViewportClient->EngineShowFlags)
			.SetRealtimeUpdate(true));
		FVector ViewLocation;
		FRotator ViewRotation;
		FSceneView* SceneView = LocalPlayer->CalcSceneView(&ViewFamily, ViewLocation, ViewRotation, LocalPlayer->ViewportClient->Viewport);

		if (SceneView)
		{
			// Transform our world position into projection coordinates.
			auto ProjPosAug = SceneView->ViewMatrices.GetProjectionMatrix().TransformPosition(WorldPos);
			FVector ProjPos(ProjPosAug);
			
			// Divide by the augmented coord W value.
			ProjPos /= ProjPosAug.W;
			
			// Clamp position to -1,1 on x and y (corresponds to on-screen)
			ProjPos.X = FMath::Clamp<float>(ProjPos.X, -1, 1);
			ProjPos.Y = FMath::Clamp<float>(ProjPos.Y, -1, 1);
			
			// Invert the transform back to world space.
			auto AugWorldSnapped = SceneView->ViewMatrices.GetInvProjectionMatrix().TransformPosition(ProjPos);
			FVector SnapPos(AugWorldSnapped);
			SnapPos /= AugWorldSnapped.W;
			*OutSnappedPos = SnapPos;
			return true;
		}
	}
	return false;
}

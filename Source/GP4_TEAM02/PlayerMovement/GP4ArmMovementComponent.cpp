
#include "GP4ArmMovementComponent.h"
#include "GP4ArmForceCalculator.h"

// Sets default values for this component's properties
UGP4ArmMovementComponent::UGP4ArmMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	ArmForceCalculator = CreateDefaultSubobject<UGP4ArmForceCalculator>("ForceCalculator");
}

void UGP4ArmMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	ArmForceCalculator->Init(&MovementConfig);
}

void UGP4ArmMovementComponent::GetLeftHandMovement(FVector Input, FVector HandPosition, bool bIsInFocusMode,
	FVector& NewPosition)
{
	FVector HandWorldPos = ArmsMesh->GetSocketLocation("hand_l");
	
	NewPosition.X = HandPosition.X;
	NewPosition.Y = ArmForceCalculator->CalculateHorizontalMovement(Input.Y, HandPosition.Y,
		bIsInFocusMode, false);
	NewPosition.Z = ArmForceCalculator->CalculateVerticalMovement(Input.X, HandPosition.Z,
		bIsInFocusMode);
}

void UGP4ArmMovementComponent::GetRightHandMovement(FVector Input, FVector HandPosition, bool bIsInFocusMode,
	FVector& NewPosition)
{
	FVector HandWorldPos = ArmsMesh->GetSocketLocation("hand_r");
	
	NewPosition.X = HandPosition.X;
	NewPosition.Y = ArmForceCalculator->CalculateHorizontalMovement(Input.Y, HandPosition.Y,
		bIsInFocusMode, true);
	NewPosition.Z = ArmForceCalculator->CalculateVerticalMovement(Input.X, HandPosition.Z,
		bIsInFocusMode);
	
	//ArmForceCalculator->SnapToViewFrustum(HandWorldPos, &NewPosition);

	// FRotator rot;
	//NewPosition = UKismetMathLibrary::InverseTransformLocation(ArmsMesh->GetComponentTransform(), NewPosition);
	
	// const FMatrix LocalToWorldInverse = ArmsMesh->GetComponentToWorld().ToMatrixWithScale().Inverse();
	// NewPosition = LocalToWorldInverse.InverseTransformPosition(NewPosition);
	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Black, FString::Printf(TEXT("newpos x: %f"), NewPosition.X));
	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Black, FString::Printf(TEXT("newpos y: %f"), NewPosition.Y));
	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Black, FString::Printf(TEXT("newpos z: %f"), NewPosition.Z));
}



#pragma once

#include "GP4GrabableItemsInterface.h"
#include "Components/ActorComponent.h"
#include "Components/BoxComponent.h"
#include "GP4_TEAM02/Gravity/GravityCharecter/PlayerPawn.h"
#include "HandsComponent.generated.h"

UENUM(BlueprintType)
enum class EHandType : uint8 {
	E_Left UMETA(DisplayName="Left Hand"),
	E_Right UMETA(DisplayName="Right Hand"),
};

UENUM(BlueprintType)
enum class EGrabableItemState : uint8 {
	E_None UMETA(DisplayName="NO State"),
	E_Highlighted UMETA(DisplayName="Item Highlighted"),
	E_Moving UMETA(DisplayName="Item Moving Towards Hand"),
	E_Holding UMETA(DisplayName="Item Is Being Held"),
};

USTRUCT()
struct FGrabableItem
{
	GENERATED_BODY()

	EGrabableItemState GrabableItemState;
	FVector MoveStartLocation;
	FRotator MoveStartRotation;
	UPROPERTY()
	AActor* Item;

	FGrabableItem()
	{
		GrabableItemState = EGrabableItemState::E_None;
		Item = nullptr;
		MoveStartLocation = {};
		MoveStartRotation = {};
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDropItem, const EHandType, HandType, AActor*, Actor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGrabItem, const EHandType, HandType, AActor*, Actor);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GP4_TEAM02_API UHandsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHandsComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FCollisionQueryParams TraceParams;
	FCollisionResponseParams Response;
	
	UPROPERTY()
	USkeletalMeshComponent* SkeletalMeshArms;
	UPROPERTY(EditAnywhere)
	FName LeftHandSocketName;
	UPROPERTY(EditAnywhere)
	FName RightHandSocketName;
	UPROPERTY(EditAnywhere)
	float GrabRadiusDistance;
	UPROPERTY(EditAnywhere)
	float ForceWhenDropItem;
	
	UFUNCTION(BlueprintCallable)
	void GrabItem(const EHandType HandType);
	UFUNCTION(BlueprintCallable)
	void ReleaseGrip(const EHandType HandType);
	UFUNCTION(BlueprintPure)
	bool HasItemInHand(const EHandType HandType) const;
	UFUNCTION(BlueprintPure)
	AActor* GetItem(const EHandType HandType) const;
	UFUNCTION(BlueprintCallable)
	void SetHandDirection(const EHandType HandType, FVector MoveDirection);
	
	UPROPERTY(BlueprintAssignable)
	FOnDropItem OnDropItem;
	UPROPERTY(BlueprintAssignable)
	FOnGrabItem OnGrabItem;
protected:
	virtual void BeginPlay() override;
private:
	UPROPERTY(EditAnywhere)
	float MaxLerpSeconds;
	float CurrentLerpTimeLeftHand;
	float CurrentLerpTimeRightHand;
	
	UPROPERTY(EditAnywhere)
	bool bDebug;
	
	bool bIsTryingToInteractLeftHand;
	bool bIsTryingToInteractRightHand;

	const FName HANDLE_SOCKET_NAME_L = "handle_l";
	const FName HANDLE_SOCKET_NAME_R = "handle_r";

	FVector LeftHandMoveDirection;
	FVector RightHandMoveDirection;
	
	UPROPERTY()
	FGrabableItem ItemForLeftHand;
	UPROPERTY()
	FGrabableItem ItemForRightHand;
	
	void TryFindNearbyItems(EHandType HandType);
	void TryGrabItem(const EHandType HandType);
	void TryDropItem(EHandType& HandType);
	void MoveTowardsHand(const EHandType HandType);
	void TryAddHighlight(const EHandType HandType, AActor* GrablableItem);
	void TryRemoveHighlight(const EHandType HandType);
	void SetCurrentLerpTime(const EHandType HandType, float LerpValue);
	void SetItem(const EHandType, AActor* Item);
	void SetItemState(const EHandType HandType, EGrabableItemState ItemState);
	void SetMoveStartLocation(const EHandType HandType, FVector Location);
	void SetMoveStartRotation(const EHandType HandType, FRotator Rotation);
	void SetGrabInput(const EHandType HandType, bool bIsTryingToGrab);

	bool IsValidGrabItem(AActor* GrabItem);
	bool IsTryingToInteract(const EHandType HandType) const;
	bool IsTryingToGrab(const EHandType HandType) const;
	bool IsTryingToDrop(const EHandType HandType) const;
	float GetCurrentLerpTime(const EHandType HandType) const;
	FName GetHandSocketName(const EHandType HandType) const;
	FName GetHandleSocketName(const EHandType HandType) const;
	EGrabableItemState GetItemState(const EHandType HandType) const;
	FGrabableItem GetGrabable(const EHandType HandType) const;
	FVector GetHandSocketLocation(const EHandType HandType) const;
	FVector GetArmMovingDirection(const EHandType HandType) const;
	FVector GetMoveStartLocation(const EHandType HandType) const;
	FQuat GetSocketQuat(const EHandType HandType) const;
	FRotator GetMoveStartRotation(const EHandType handType) const;
	AActor* SearchForItem(const EHandType HandType) const;
	FHitResult SweepHand(FVector Start, FVector End) const;
};

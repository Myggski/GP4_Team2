#include "HandsComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/GameSession.h"
#include "Kismet/GameplayStatics.h"

UHandsComponent::UHandsComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bDebug = false;

	bIsTryingToInteractLeftHand = false;
	bIsTryingToInteractRightHand = false;

	ItemForLeftHand = {};
	ItemForRightHand = {};
	
	LeftHandSocketName = "middle_metacarpal_l";
	RightHandSocketName = "middle_metacarpal_r";

	SkeletalMeshArms = nullptr;
	GrabRadiusDistance = 150.f;
	ForceWhenDropItem = 600.f;

	MaxLerpSeconds = 0.2f;
	CurrentLerpTimeLeftHand = 0.f;
	CurrentLerpTimeRightHand = 0.f;

	TraceParams.AddIgnoredActor(GetOwner());
}

void UHandsComponent::BeginPlay()
{
	Super::BeginPlay();

	SkeletalMeshArms = Cast<USkeletalMeshComponent>(GetOwner()->GetComponentByClass(USkeletalMeshComponent::StaticClass()));

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerPawn::StaticClass(), FoundActors);
	TraceParams.AddIgnoredActors(FoundActors);
}

/**
 * @brief Loops through each hand and checks the states of the items nearby the hands
 * If there's a item nearby it will try to do certain actions.
 * If there''s no items, the hands will try to search for items. 
 */
void UHandsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	for (EHandType HandType : { EHandType::E_Left, EHandType::E_Right })
	{
		const EGrabableItemState ItemState = GetItemState(HandType);

		// If there's no item, search for item
		if (ItemState == EGrabableItemState::E_None)
		{
			TryFindNearbyItems(HandType);
		} else if (ItemState == EGrabableItemState::E_Highlighted)
		{
			AActor* FoundItem = SearchForItem(HandType);
			// If item was once in reach but not anymore, the highlight needs to be removed
			if (GetItem(HandType) != FoundItem)
			{
				TryRemoveHighlight(HandType);
			}
			
			if (FoundItem)
			{
				TryGrabItem(HandType);
			}
		} else if (ItemState == EGrabableItemState::E_Moving)
		{
			MoveTowardsHand(HandType);
		} else if (ItemState == EGrabableItemState::E_Holding)
		{
			// If the hand is holding an item, check if player wants to drop it
			TryDropItem(HandType);

			// If the item has been destroyed while holding it, remove it from the hand
			if (!IsValid(GetItem(HandType)))
			{
				SetItemState(HandType, EGrabableItemState::E_None);
				SetItem(HandType, nullptr);
				SetMoveStartLocation(HandType, {});	
			}
		}
	}
}

/**
 * @brief Is trying to interact with the selected hand
 * @param HandType Left or right hand
 */
void UHandsComponent::GrabItem(const EHandType HandType)
{
	SetGrabInput(HandType, true);
}

/**
 * @brief Is trying to release the grip with the selected hand
 * @param HandType Left or right hand
 */
void UHandsComponent::ReleaseGrip(const EHandType HandType)
{
	SetGrabInput(HandType, false);
}

/**
 * @brief Checks if there's an item in the selected hand
 * @param HandType Left or right hand 
 * @return True if there's an item in the hand else it returns false
 */
bool UHandsComponent::HasItemInHand(const EHandType HandType) const
{
	return GetItem(HandType) && GetItemState(HandType) == EGrabableItemState::E_Holding;
}

/**
 * @brief Returns the item in the hand, can return null
 * @param HandType Left or right hand
 * @return The held item or null
 */
AActor* UHandsComponent::GetItem(const EHandType HandType) const
{
	return GetGrabable(HandType).Item;
}

/**
 * @brief This is being called in Player2Pawn blueprint
 * @param HandType Left or right hand
 * @param MoveDirection What direction is the select hand moving
 */
void UHandsComponent::SetHandDirection(const EHandType HandType, FVector MoveDirection)
{
	if (HandType == EHandType::E_Left)
	{
		LeftHandMoveDirection = MoveDirection;
	} else
	{
		RightHandMoveDirection = MoveDirection;	
	}
}

/**
 * @brief Trying to find nearby items and highlight the nearest
 * @param HandType Left or Right hand
 */
void UHandsComponent::TryFindNearbyItems(EHandType HandType)
{
	AActor* FoundItem = SearchForItem(HandType);

	// If item is found, highlight the item
	if (IsValidGrabItem(FoundItem))
	{
		TryAddHighlight(HandType, FoundItem);
	} else if (IsTryingToInteract(HandType))
	{
		ReleaseGrip(HandType);
	}
}

void UHandsComponent::TryGrabItem(const EHandType HandType)
{
	if (IsTryingToGrab(HandType))
	{
		AActor* GrabItem = GetItem(HandType);

		for (UActorComponent* ActorComponent : GrabItem->GetComponents())
		{
			UPrimitiveComponent* Primitive = Cast<UPrimitiveComponent>(ActorComponent);

			if (Primitive)
			{
				Primitive->SetEnableGravity(false);

				if (Primitive->IsCollisionEnabled())
				{
					Primitive->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
					Primitive->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
					Primitive->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
				}

				if (Primitive == GrabItem->GetRootComponent())
				{
					Primitive->SetSimulatePhysics(false);	
				}
			}
		}
		
		SetItemState(HandType, EGrabableItemState::E_Moving);
		SetMoveStartLocation(HandType, GetItem(HandType)->GetActorLocation());
		SetMoveStartRotation(HandType, GetItem(HandType)->GetActorRotation());
	}
}

void UHandsComponent::TryDropItem(EHandType& HandType)
{
	if (IsTryingToDrop(HandType))
	{
		AActor* HeldItem = GetItem(HandType);

		HeldItem->DetachFromActor(FDetachmentTransformRules(
			EDetachmentRule::KeepWorld,
			EDetachmentRule::KeepWorld,
			EDetachmentRule::KeepWorld,
			true
		));
		
		for (UActorComponent* ActorComponent : HeldItem->GetComponents())
		{
			UPrimitiveComponent* Primitive = Cast<UPrimitiveComponent>(ActorComponent);

			if (Primitive)
			{
				Primitive->SetEnableGravity(true);

				if (Primitive->IsCollisionEnabled())
				{
					Primitive->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
					Primitive->SetCollisionResponseToChannel(ECC_Camera, ECR_Block);
					Primitive->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);
				}

				if (Primitive == HeldItem->GetRootComponent())
				{
					Primitive->SetSimulatePhysics(true);
					Primitive->AddImpulse(GetArmMovingDirection(HandType) * ForceWhenDropItem);
				}

				if (Primitive->IsSimulatingPhysics())
				{
					// Resets Velociity
					Primitive->SetAllPhysicsLinearVelocity(FVector::Zero());
					Primitive->SetAllPhysicsAngularVelocityInDegrees(FVector::Zero());
				}
			}
		}

		SetItemState(HandType, EGrabableItemState::E_None);
		SetItem(HandType, nullptr);
		SetMoveStartLocation(HandType, {});
		OnDropItem.Broadcast(HandType, HeldItem);
	}
}

void UHandsComponent::MoveTowardsHand(const EHandType HandType)
{
	AActor* Item = GetItem(HandType);
	float CurrentLerpTime = GetCurrentLerpTime(HandType);
	const float LerpT = CurrentLerpTime / MaxLerpSeconds < 1.f ? CurrentLerpTime / MaxLerpSeconds : 1.f;

	// Rotation
	FQuat LocalHandleRotation = Item->GetTransform().InverseTransformRotation(IGP4GrabableItemsInterface::Execute_GetGrabRotation(Item, GetHandleSocketName(HandType)).Quaternion());
	FQuat EndRotaton = SkeletalMeshArms->GetSocketTransform(GetHandSocketName(HandType)).InverseTransformRotation(LocalHandleRotation);
	const FQuat SlerpRotation = FQuat::Slerp(GetMoveStartRotation(HandType).Quaternion(), EndRotaton, LerpT);
	Item->SetActorRotation(SlerpRotation);

	// Location
	FVector ItemHandleLocalLocation = IGP4GrabableItemsInterface::Execute_GetGrabOffsetLocation(Item, GetHandleSocketName(HandType));
	FVector ItemHandleLocalRotated = EndRotaton.RotateVector(ItemHandleLocalLocation);
	Item->SetActorLocation(FMath::Lerp(GetMoveStartLocation(HandType),GetHandSocketLocation(HandType) - ItemHandleLocalRotated, LerpT));

	if (CurrentLerpTime >= MaxLerpSeconds)
	{
		Item->AttachToComponent(SkeletalMeshArms, FAttachmentTransformRules(
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::KeepWorld,
			true),
			GetHandSocketName(HandType)
		);

		FQuat HandleWorldRotation = IGP4GrabableItemsInterface::Execute_GetGrabRotation(Item, GetHandleSocketName(HandType)).Quaternion();
		Item->AddActorLocalRotation(SkeletalMeshArms->GetSocketTransform(GetHandSocketName(HandType)).InverseTransformRotation(-HandleWorldRotation));
		Item->AddActorLocalOffset(-ItemHandleLocalLocation);
		
		OnGrabItem.Broadcast(HandType, Item);
		SetMoveStartLocation(HandType, {});
		SetItemState(HandType, EGrabableItemState::E_Holding);
		SetCurrentLerpTime(HandType, 0.f);
	} else
	{
		SetCurrentLerpTime(HandType, CurrentLerpTime + GetWorld()->DeltaTimeSeconds);	
	}
}

void UHandsComponent::TryAddHighlight(const EHandType HandType, AActor* GrablableItem)
{
	if (GrablableItem && !GetItem(HandType))
	{
		IGP4GrabableItemsInterface::Execute_AddHighlight(GrablableItem);
	}

	if (GetItem(HandType) != GrablableItem)
	{
		TryRemoveHighlight(HandType);
		SetItem(HandType, GrablableItem);
		SetItemState(HandType, EGrabableItemState::E_Highlighted);
	}
}

void UHandsComponent::TryRemoveHighlight(const EHandType HandType)
{
	AActor* Item = GetItem(HandType);

	if (Item && GetItemState(HandType) == EGrabableItemState::E_Highlighted)
	{
		IGP4GrabableItemsInterface::Execute_RemoveHighlight(Item);
	}
	
	SetItemState(HandType, EGrabableItemState::E_None);
	SetItem(HandType, nullptr);
	SetMoveStartLocation(HandType, {});
}

void UHandsComponent::SetCurrentLerpTime(const EHandType HandType, float LerpValue)
{
	if (HandType == EHandType::E_Left)
	{
		CurrentLerpTimeLeftHand = LerpValue;
	} else
	{
		CurrentLerpTimeRightHand = LerpValue;
	}
}

void UHandsComponent::SetItem(const EHandType HandType, AActor* Item)
{
	if (HandType == EHandType::E_Left)
	{
		ItemForLeftHand.Item = Item;
	} else
	{
		ItemForRightHand.Item = Item;
	}
}

void UHandsComponent::SetItemState(const EHandType HandType, EGrabableItemState ItemState)
{
	if (HandType == EHandType::E_Left)
	{
		ItemForLeftHand.GrabableItemState = ItemState;
	} else
	{
		ItemForRightHand.GrabableItemState = ItemState;
	}
}

void UHandsComponent::SetMoveStartLocation(const EHandType HandType, FVector Location)
{
	if (HandType == EHandType::E_Left)
	{
		ItemForLeftHand.MoveStartLocation = Location;
	} else
	{
		ItemForRightHand.MoveStartLocation = Location;
	}
}

void UHandsComponent::SetMoveStartRotation(const EHandType HandType, FRotator Rotation)
{
	if (HandType == EHandType::E_Left)
	{
		ItemForLeftHand.MoveStartRotation = Rotation;
	} else
	{
		ItemForRightHand.MoveStartRotation = Rotation;
	}
}

void UHandsComponent::SetGrabInput(const EHandType HandType, const bool bIsTryingToGrab)
{
	if (HandType == EHandType::E_Left)
	{
		bIsTryingToInteractLeftHand = bIsTryingToGrab;
	} else
	{
		bIsTryingToInteractRightHand = bIsTryingToGrab;
	}
}

/**
 * @brief Checks if an Actor has the grabable interface or not implemented
 * @param GrabItem Actor in the game world
 * @return Returns true if it has the interface, otherwise false
 */
bool UHandsComponent::IsValidGrabItem(AActor* GrabItem)
{
	return GrabItem && GrabItem->GetClass()->ImplementsInterface(UGP4GrabableItemsInterface::StaticClass());
}

bool UHandsComponent::IsTryingToInteract(const EHandType HandType) const
{
	if (HandType == EHandType::E_Left)
	{
		return bIsTryingToInteractLeftHand;
	}

	return bIsTryingToInteractRightHand;
}

bool UHandsComponent::IsTryingToGrab(const EHandType HandType) const
{
	return IsTryingToInteract(HandType) && !HasItemInHand(HandType);
}

bool UHandsComponent::IsTryingToDrop(const EHandType HandType) const
{
	if (HandType == EHandType::E_Left)
	{
		return !bIsTryingToInteractLeftHand && GetItemState(HandType) == EGrabableItemState::E_Holding;
	}

	return !bIsTryingToInteractRightHand && GetItemState(HandType) == EGrabableItemState::E_Holding;
}

float UHandsComponent::GetCurrentLerpTime(const EHandType HandType) const
{
	if (HandType == EHandType::E_Left)
	{
		return CurrentLerpTimeLeftHand;
	}

	return CurrentLerpTimeRightHand;;
}

FName UHandsComponent::GetHandSocketName(const EHandType HandType) const
{
	if (HandType == EHandType::E_Left)
	{
		return LeftHandSocketName;	
	}

	return RightHandSocketName;
}

FName UHandsComponent::GetHandleSocketName(const EHandType HandType) const
{
	if (HandType == EHandType::E_Left)
	{
		return HANDLE_SOCKET_NAME_L;
	}

	return HANDLE_SOCKET_NAME_R;
}


EGrabableItemState UHandsComponent::GetItemState(const EHandType HandType) const
{
	return GetGrabable(HandType).GrabableItemState;
}

FGrabableItem UHandsComponent::GetGrabable(const EHandType HandType) const
{
	if (HandType == EHandType::E_Left)
	{
		return ItemForLeftHand;
	}

	return ItemForRightHand;
}

FVector UHandsComponent::GetHandSocketLocation(const EHandType HandType) const
{
	return SkeletalMeshArms->GetSocketLocation(GetHandSocketName(HandType));
}

FVector UHandsComponent::GetArmMovingDirection(const EHandType HandType) const
{
	if (HandType == EHandType::E_Left)
	{
		return GetOwner()->GetActorRotation().RotateVector(LeftHandMoveDirection);
	}

	return GetOwner()->GetActorRotation().RotateVector(RightHandMoveDirection);
}

FVector UHandsComponent::GetMoveStartLocation(const EHandType HandType) const
{
	return GetGrabable(HandType).MoveStartLocation;
}

FQuat UHandsComponent::GetSocketQuat(const EHandType HandType) const
{
	return SkeletalMeshArms->GetSocketQuaternion(GetHandSocketName(HandType));
}


FRotator UHandsComponent::GetMoveStartRotation(const EHandType HandType) const
{
	return GetGrabable(HandType).MoveStartRotation;
}

/**
 * @brief Searching for items to grab, highlighting the items and see if the player is trying to grab them
 * @param HandType Left or Right hand
 * @return Returns actor that is in the Grabitem trace chanel
 */
AActor* UHandsComponent::SearchForItem(const EHandType HandType) const
{
	const FVector StartAndEnd = GetHandSocketLocation(HandType);
	FHitResult Hit = SweepHand(StartAndEnd, StartAndEnd);
	
	return Hit.GetActor();
}

FHitResult UHandsComponent::SweepHand(FVector Start, FVector End) const
{
	TArray<FHitResult> Hits;
	FHitResult ClosestHit = FHitResult(-1);

	if (bDebug)
	{
		DrawDebugSphere( GetWorld(), Start, GrabRadiusDistance, 32, FColor::Red, false, 0.1f );	
	}
	
	GetWorld()->SweepMultiByChannel(
		Hits,
		Start,
		End,
		FQuat::Identity,
		ECC_GameTraceChannel1, // <-- Bara grabable items
		FCollisionShape::MakeSphere(GrabRadiusDistance),
		TraceParams
	);

	for (FHitResult Hit : Hits)
	{
		float HitDistance = FVector::Distance(Hit.GetActor()->GetActorLocation(), Start);
		float HitClosest = ClosestHit.GetActor() ? FVector::Distance(ClosestHit.GetActor()->GetActorLocation(), Start) : -1;
		if (HitDistance < HitClosest || HitClosest == -1)
		{
			ClosestHit = Hit;
		}
	}

	return ClosestHit;
}


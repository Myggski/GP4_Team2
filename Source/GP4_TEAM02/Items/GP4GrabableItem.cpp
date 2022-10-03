#include "GP4GrabableItem.h"

AGP4GrabableItem::AGP4GrabableItem()
{
	PrimaryActorTick.bCanEverTick = true;
	
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	StaticMeshComponent->SetSimulatePhysics(true);
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	StaticMeshComponent->SetCollisionResponseToAllChannels(ECR_Block);
	StaticMeshComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Ignore);
	StaticMeshComponent->OnComponentSleep.AddDynamic(this, &AGP4GrabableItem::OnComponentSleep);
	SetRootComponent(StaticMeshComponent);
}

void AGP4GrabableItem::BeginPlay()
{
	Super::BeginPlay();
}

void AGP4GrabableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGP4GrabableItem::OnComponentSleep(UPrimitiveComponent* SleepingComponent, FName BoneName)
{
	SleepingComponent->WakeAllRigidBodies();
}

void AGP4GrabableItem::AddHighlight_Implementation()
{
	StaticMeshComponent->SetRenderCustomDepth(true);
}

void AGP4GrabableItem::RemoveHighlight_Implementation()
{
	StaticMeshComponent->SetRenderCustomDepth(false);
}

FVector AGP4GrabableItem::GetGrabOffsetLocation_Implementation(FName HandleSocketName)
{
	FVector SocketWorldLocation = StaticMeshComponent->GetSocketLocation(HandleSocketName);

	if (SocketWorldLocation == FTransform::Identity.GetLocation())
	{
		SocketWorldLocation = StaticMeshComponent->GetSocketLocation(DEFAULT_HANDLE_NAME);
	}

	return StaticMeshComponent->GetOwner()->GetActorTransform().InverseTransformPosition(SocketWorldLocation);
}

FRotator AGP4GrabableItem::GetGrabRotation_Implementation(FName HandleSocketName)
{
	FRotator SocketRotation = StaticMeshComponent->GetSocketRotation(HandleSocketName);

	if (SocketRotation == FTransform::Identity.GetRotation().Rotator())
	{
		SocketRotation = StaticMeshComponent->GetSocketRotation(DEFAULT_HANDLE_NAME);
	}

	return SocketRotation;
}



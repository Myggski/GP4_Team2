// Fill out your copyright notice in the Description page of Project Settings.


#include "GP4PortalComponent.h"

#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UGP4PortalComponent::UGP4PortalComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UGP4PortalComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UGP4PortalComponent::LoadConnectedLevel()
{
	const FLatentActionInfo LatentInfo;
	UGameplayStatics::LoadStreamLevelBySoftObjectPtr(this, DestinationLevel, true, true, LatentInfo);
}

void UGP4PortalComponent::Teleport()
{
	
}


// Called every frame
void UGP4PortalComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}


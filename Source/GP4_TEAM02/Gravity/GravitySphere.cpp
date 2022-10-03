// Fill out your copyright notice in the Description page of Project Settings.


#include "GravitySphere.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/GameEngine.h"

AGravitySphere::AGravitySphere()
{
	PrimaryActorTick.bCanEverTick = true;

	GravitySphere = CreateDefaultSubobject<USphereComponent>(TEXT("Planet"));
	RootComponent = GravitySphere;

	GravitySphere->OnComponentBeginOverlap.AddDynamic(this, &AGravitySphere::OnOverlapBegin);
	GravitySphere->OnComponentEndOverlap.AddDynamic(this, &AGravitySphere::OnOverlapEnd);  
}

void AGravitySphere::BeginPlay()
{
	Super::BeginPlay();
}

void AGravitySphere::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	for (auto Component : ComponentArray)
	{
		Component->AddRadialForce(GetActorLocation(), GravitySphere->GetScaledSphereRadius(), -GravityStrength, ERadialImpulseFalloff::RIF_Constant, true);
	}
}

void AGravitySphere::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (DebugInsideGravitySphere)
	{
		GEngine->AddOnScreenDebugMessage (-1, 15.0f, FColor::Green, TEXT ("Inside Gravity Sphere!"));
	}
			
	if (OtherActor && (OtherActor != this) && OtherComp->IsSimulatingPhysics())
	{
		ComponentArray.Add(OtherComp);
	}
}

void AGravitySphere::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		if (DebugInsideGravitySphere)
		{
			GEngine->AddOnScreenDebugMessage (-1, 15.0f, FColor::Red, TEXT ("Exit Gravity Sphere!"));
		}
			
		ComponentArray.Remove(OtherComp);
	}
}




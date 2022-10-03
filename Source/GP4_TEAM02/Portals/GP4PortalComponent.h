// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GP4PortalComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GP4_TEAM02_API UGP4PortalComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UGP4PortalComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UWorld> DestinationLevel;
	
	UPROPERTY(EditAnywhere)
	UGP4PortalComponent* ConnectedPortal;
	
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* PortalMesh;
	
	UPROPERTY(VisibleAnywhere)
	bool bIsOpen;

protected:
	virtual void BeginPlay() override;


private:
	void LoadConnectedLevel();
	void Teleport();
};

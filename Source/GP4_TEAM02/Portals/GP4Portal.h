#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GP4Portal.generated.h"

UCLASS()
class GP4_TEAM02_API AGP4Portal : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGP4Portal();
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	AGP4Portal* ConnectedPortal;
	
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* PortalMesh;
	
	UPROPERTY(VisibleAnywhere)
	bool bIsOpen;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void Teleport();
};

#include "GP4Portal.h"


// Sets default values
AGP4Portal::AGP4Portal()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetRootComponent(PortalMesh);
}

// Called when the game starts or when spawned
void AGP4Portal::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGP4Portal::Teleport()
{
}



// Called every frame
void AGP4Portal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


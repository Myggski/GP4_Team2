#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GP4GrabableItemsInterface.generated.h"

UINTERFACE(MinimalAPI)
class UGP4GrabableItemsInterface : public UInterface
{
	GENERATED_BODY()
};

class GP4_TEAM02_API IGP4GrabableItemsInterface
{
	GENERATED_BODY()
public:
	// Add highlight from the object, to display that you can grab the item
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void AddHighlight();

	// Removes the highlight from the grabable object
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void RemoveHighlight();

	// Returns the "handle" location of the object to grab.
	// Relative/Local location, not world location
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FVector GetGrabOffsetLocation(FName HandleSocketName);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FRotator GetGrabRotation(FName HandleSocketName);
};

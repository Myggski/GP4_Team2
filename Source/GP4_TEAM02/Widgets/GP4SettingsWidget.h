
#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GP4SettingsWidget.generated.h"

DECLARE_DYNAMIC_DELEGATE(FController1ConfigChanged);
DECLARE_DYNAMIC_DELEGATE(FController2ConfigChanged);

UCLASS()
class GP4_TEAM02_API UGP4SettingsWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnController1ConfigChanged() const;
	UFUNCTION()
	void OnController2ConfigChanged() const;

	UPROPERTY()
	FController1ConfigChanged OnController1ConfigUpdated;
	UPROPERTY()
	FController2ConfigChanged OnController2ConfigUpdated;
};

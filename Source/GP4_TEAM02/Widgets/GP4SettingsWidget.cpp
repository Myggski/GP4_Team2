
#include "GP4SettingsWidget.h"

void UGP4SettingsWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UGP4SettingsWidget::OnController1ConfigChanged() const
{
	OnController1ConfigUpdated.Execute();
}

void UGP4SettingsWidget::OnController2ConfigChanged() const
{
	OnController2ConfigUpdated.Execute();
}

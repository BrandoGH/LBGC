// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingWidget.h"
#include <Components/ComboBoxString.h>

bool USettingWidget::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	m_combWindowsMode = Cast<UComboBoxString>(GetWidgetFromName("ComboBoxWindowsMode"));

	DefaultSetting();
	return true;
}

int32 USettingWidget::GetWindowsMode()
{
	if (!m_combWindowsMode)
	{
		return Fullscreen;
	}
	return m_combWindowsMode->GetSelectedIndex();
}

void USettingWidget::DefaultSetting()
{
	if (!m_combWindowsMode)
	{
		return;
	}
	m_combWindowsMode->SetSelectedIndex(Fullscreen);
}

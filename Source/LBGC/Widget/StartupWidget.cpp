// Fill out your copyright notice in the Description page of Project Settings.

#ifdef LBGS_DEBUG
PRAGMA_DISABLE_OPTIMIZATION
#endif // LBGS_DEBUG
#include "StartupWidget.h"
#include <Components/TextBlock.h>
#include <Components/Throbber.h>

bool UStartupWidget::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	m_textTip = Cast<UTextBlock>(GetWidgetFromName("TextBlock_Tip"));
	m_waiting = Cast<UThrobber>(GetWidgetFromName("Throbber_Waiting"));

	DefaultSetting();
	return true;
}

void UStartupWidget::SetTip(const FString& str)
{
	if (m_textTip)
	{
		m_textTip->SetText(FText::FromString(str));
	}
}

void UStartupWidget::SetWaitngShow(bool bShow)
{
	if (m_waiting)
	{
		m_waiting->SetVisibility(bShow ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

void UStartupWidget::DefaultSetting()
{
	SetTip(TEXT("connecting to server........"));
	SetWaitngShow(true);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginWidget.h"
#include <Components/Button.h>
#include <Components/CircularThrobber.h>
#include <Components/TextBlock.h>
#include <Components/EditableTextBox.h>

bool ULoginWidget::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	m_btLogin = Cast<UButton>(GetWidgetFromName("Button_Login"));
	m_loading = Cast<UCircularThrobber>(GetWidgetFromName("CircularThrobber_Loading"));
	m_textTip = Cast<UTextBlock>(GetWidgetFromName("TextBlock_Tip"));
	m_editRoleName = Cast<UEditableTextBox>(GetWidgetFromName("EditableTextBox_RoleName"));
	m_editRolePassword = Cast<UEditableTextBox>(GetWidgetFromName("EditableTextBox_Password"));

	DefaultSetting();
	return true;
}

void ULoginWidget::SetLoginButtonEnable(bool bEnable)
{
	if (m_btLogin)
	{
		m_btLogin->SetIsEnabled(bEnable);
	}
}

void ULoginWidget::SetLoadingShow(bool bShow)
{
	if (m_loading)
	{
		m_loading->SetVisibility(bShow ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

void ULoginWidget::SetTip(const FString& str)
{
	if (m_textTip)
	{
		m_textTip->SetText(FText::FromString(str));
	}
}

FString ULoginWidget::GetRoleName()
{
	if (m_editRoleName)
	{
		return m_editRoleName->GetText().ToString();
	}
	return FString();
}

FString ULoginWidget::GetRolePassword()
{
	if (m_editRolePassword)
	{
		return m_editRolePassword->GetText().ToString();
	}
	return FString();
}

void ULoginWidget::DefaultSetting()
{
	SetLoginButtonEnable(false);
	SetLoadingShow(false);
}

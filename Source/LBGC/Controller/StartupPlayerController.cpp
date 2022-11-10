// Fill out your copyright notice in the Description page of Project Settings.

#ifdef LBGS_DEBUG
PRAGMA_DISABLE_OPTIMIZATION
#endif // LBGS_DEBUG
#include "StartupPlayerController.h"
#include "../Widget/StartupWidget.h"
#include "../Widget/LoginWidget.h"


AStartupPlayerController::AStartupPlayerController()
	: m_HUDStartup(NULL)
	, m_HUDLogin(NULL)
{

}

void AStartupPlayerController::BeginPlay()
{
	Super::BeginPlay();
	InitFromBeginPlay();
}

void AStartupPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AStartupPlayerController::SwitchToView(EnSwitchHUD target)
{
	switch (target)
	{
	case EnSwitchHUD::ESH_STARTUP:
		SwitchHUDToStartUp();
		break;
	case EnSwitchHUD::ESH_LOGIN:
		SwitchHUDToLogin();
		break;
	}
}

void AStartupPlayerController::OnlineClick()
{
	SwitchToView(EnSwitchHUD::ESH_LOGIN);
}

void AStartupPlayerController::InitFromBeginPlay()
{
	if (!HUDStartupClass || !HUDLoginClass)
	{
		return;
	}
	m_HUDStartup = CreateWidget<UStartupWidget>(this, HUDStartupClass);
	m_HUDStartup->AddToViewport();
	m_HUDLogin = CreateWidget<ULoginWidget>(this, HUDLoginClass);
	m_HUDLogin->AddToViewport();
	SwitchToView(EnSwitchHUD::ESH_STARTUP);
	SetShowMouseCursor(true);
}

void AStartupPlayerController::SwitchHUDToStartUp()
{
	if (!m_HUDStartup ||
		!m_HUDLogin)
	{
		return;
	}

	m_HUDStartup->SetVisibility(ESlateVisibility::Visible);
	m_HUDLogin->SetVisibility(ESlateVisibility::Hidden);
}

void AStartupPlayerController::SwitchHUDToLogin()
{
	if (!m_HUDStartup ||
		!m_HUDLogin)
	{
		return;
	}

	m_HUDLogin->SetVisibility(ESlateVisibility::Visible);
	m_HUDStartup->SetVisibility(ESlateVisibility::Hidden);
}

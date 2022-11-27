// Fill out your copyright notice in the Description page of Project Settings.

#ifdef LBGS_DEBUG
PRAGMA_DISABLE_OPTIMIZATION
#endif // LBGS_DEBUG
#include "StartupPlayerController.h"
#include "../Widget/StartupWidget.h"
#include "../Widget/LoginWidget.h"
#include "../GameInstance/LBGCGameInstance.h"
#include "../MsgModule/MsgCommon.h"
#include "../MsgModule/Msg/MsgLogin.h"
#include "../ConfigModule/StartupConfig.h"
#include <Blueprint/UserWidget.h>
#include "../Widget/SettingWidget.h"


AStartupPlayerController::AStartupPlayerController()
	: m_HUDStartup(NULL)
	, m_HUDLogin(NULL)
	, m_HUDSetting(NULL)
	, m_quieGame(false)
{

}

void AStartupPlayerController::BeginPlay()
{
	Super::BeginPlay();
	m_quieGame = false;
	InitFromBeginPlay();
	ConnectToServer();
}

void AStartupPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	m_quieGame = true;
}

void AStartupPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (LBGC_INSTANCE && LBGC_INSTANCE->GetTcpClient())
	{
		if (LBGC_INSTANCE->GetTcpClient()->IsTimerout() && m_HUDLogin)
		{
			m_HUDLogin->SetTip(TEXT("Connect Timeout, Please Check your Internet!"));
		}
	}
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
	case EnSwitchHUD::ESH_SETTING:
		SwitchHUDToSetting();
		break;
	}
}

void AStartupPlayerController::OnlineClick()
{
	SwitchToView(EnSwitchHUD::ESH_LOGIN);
}

void AStartupPlayerController::Login()
{
	if (!m_HUDLogin || !LBGC_INSTANCE)
	{
		return;
	}
	if (!CheckRoleNameSize() || !CheckRolePasswordSize())
	{
		m_HUDLogin->SetTip(TEXT("Error,Please check your role name or password length"));
		return;
	}
	m_HUDLogin->SetTip(TEXT("Logining......"));
	m_HUDLogin->SetLoadingShow(true);

	SendLoginInfo();
}

bool AStartupPlayerController::CheckRoleNameSize()
{
	if (!m_HUDLogin)
	{
		return false;
	}

	if (m_HUDLogin->GetRoleName().Len() > NSTcpClient::g_nRoleNameSize || m_HUDLogin->GetRoleName().Len() <= 0)
	{
		return false;
	}

	return true;
}

bool AStartupPlayerController::CheckRolePasswordSize()
{
	if (!m_HUDLogin)
	{
		return false;
	}

	if (m_HUDLogin->GetRolePassword().Len() > NSTcpClient::g_nRolePasswordSize || m_HUDLogin->GetRolePassword().Len() <= 0)
	{
		return false;
	}

	return true;
}

void AStartupPlayerController::OnSwitchWindowsMode()
{
	if (!m_HUDSetting || !LBGC_INSTANCE)
	{
		return;
	}

	LBGC_INSTANCE->UserSettingSetFullscreenMode((EWindowMode::Type)m_HUDSetting->GetWindowsMode());
}

void AStartupPlayerController::OnSwitchVisualEffectQuality()
{
	if (!m_HUDSetting || !LBGC_INSTANCE)
	{
		return;
	}

	LBGC_INSTANCE->UserSettingSwitchVisualEffectQuality(m_HUDSetting->GetEffectQuality());
}

void AStartupPlayerController::InitFromBeginPlay()
{
	if (!HUDStartupClass || !HUDLoginClass || !HUDSettingClass)
	{
		return;
	}
	m_HUDStartup = CreateWidget<UStartupWidget>(this, HUDStartupClass);
	m_HUDStartup->AddToViewport();
	m_HUDLogin = CreateWidget<ULoginWidget>(this, HUDLoginClass);
	m_HUDLogin->AddToViewport();
	m_HUDSetting = CreateWidget<USettingWidget>(this, HUDSettingClass);
	m_HUDSetting->AddToViewport();

	SwitchToView(EnSwitchHUD::ESH_STARTUP);
	SetShowMouseCursor(true);
}

void AStartupPlayerController::ConnectToServer()
{
	if (!LBGC_INSTANCE)
	{
		return;
	}

	UTcpClient* client =  LBGC_INSTANCE->GetTcpClient();
	UStartupConfig* config = LBGC_INSTANCE->GetStartupConfig();

	if (!client || !config)
	{
		return;
	}

	FClientConnectDelegate cd;
	cd.BindLambda(
		[&](bool ok, const FString& info)
		{
			if (m_quieGame)
			{
				return;
			}
			if (m_HUDLogin)
			{
				m_HUDLogin->SetLoginButtonEnable(ok);
			}

			if (m_HUDStartup)
			{
				m_HUDStartup->SetTip(info);
				m_HUDStartup->SetWaitngShow(false);
			}

			if (LBGC_INSTANCE->GetTcpClient())
			{
				LBGC_INSTANCE->GetTcpClient()->SetConnectedFlag(ok);
			}

			if (ok)
			{
				OnConnectServerOk();
			}
		});

	client->Connect(config->GetCfgIp(), config->GetCfgPort(), cd);
}

void AStartupPlayerController::OnConnectServerOk_Implementation()
{
	if (!LBGC_INSTANCE)
	{
		return;
	}
	if (!LBGC_INSTANCE->GetTcpClient())
	{
		return;
	}

	LBGC_INSTANCE->GetTcpClient()->StartRead();
	LBGC_INSTANCE->GetTcpClient()->StartSendHeart();
}

void AStartupPlayerController::SwitchHUDToStartUp()
{
	if (!m_HUDStartup ||
		!m_HUDLogin ||
		!m_HUDSetting)
	{
		return;
	}

	m_HUDStartup->SetVisibility(ESlateVisibility::Visible);
	m_HUDSetting->SetVisibility(ESlateVisibility::Hidden);
	m_HUDLogin->SetVisibility(ESlateVisibility::Hidden);
}

void AStartupPlayerController::SwitchHUDToLogin()
{
	if (!m_HUDStartup ||
		!m_HUDLogin ||
		!m_HUDSetting)
	{
		return;
	}

	m_HUDLogin->SetVisibility(ESlateVisibility::Visible);
	m_HUDSetting->SetVisibility(ESlateVisibility::Hidden);
	m_HUDStartup->SetVisibility(ESlateVisibility::Hidden);

	m_HUDLogin->SetTip("");
	m_HUDLogin->SetLoadingShow(false);

}

void AStartupPlayerController::SwitchHUDToSetting()
{
	if (!m_HUDStartup ||
		!m_HUDLogin ||
		!m_HUDSetting)
	{
		return;
	}

	m_HUDSetting->SetVisibility(ESlateVisibility::Visible);
	m_HUDStartup->SetVisibility(ESlateVisibility::Hidden);
	m_HUDLogin->SetVisibility(ESlateVisibility::Hidden);
}

void AStartupPlayerController::SendLoginInfo()
{
	if (!m_HUDLogin ||
		!LBGC_INSTANCE ||
		!LBGC_INSTANCE->GetTcpClient())
	{
		return;
	}
	MsgLoginCS cs;
	const char* roleName = TCHAR_TO_ANSI(*m_HUDLogin->GetRoleName());
	const char* rolePasswd = TCHAR_TO_ANSI(*m_HUDLogin->GetRolePassword());
	memmove(cs.m_strRoleName, roleName, strlen(roleName));
	memmove(cs.m_strPassword, rolePasswd, strlen(rolePasswd));

	m_dgLoginSC.BindUObject(this, &AStartupPlayerController::OnLoginSC);
	UTcpClient::ExpectMsgStruct expect;
	expect.ExpectMsgType = MSG_TYPE_LOGIN_SC;
	expect.ExpectDg = m_dgLoginSC;
	LBGC_INSTANCE->GetTcpClient()->Send((const uint8*)&cs, sizeof(MsgLoginCS), MSG_TYPE_LOGIN_CS, expect);
}

void AStartupPlayerController::OnLoginSC(const uint8* msg)
{
	MsgLoginSC* sc = (MsgLoginSC*)msg;
	if (!sc || !m_HUDLogin)
	{
		return;
	}

	if (sc->m_cLoginStatus == MsgLoginSC::LS_LOGIN_OK)
	{
		MsgLoginSC lambdaSC = *sc;
		FAyncLoadLevelDelegate callback;
		callback.BindLambda(
			[&, lambdaSC](bool ok)
			{
				if (!ok)
				{
					if (m_HUDLogin)
					{
						m_HUDLogin->SetTip(TEXT("Load level error"));
					}
					return;
				}
				SetShowMouseCursor(false);

				AMainRole* role = LBGC_INSTANCE->GetLocalRole();
				if (role)
				{
					role->SetRoleName(FString(strlen((const char*)lambdaSC.m_strRoleName), (const char*)lambdaSC.m_strRoleName));
				}

			}
		);
		LBGC_INSTANCE->LoadMainGameLevel(callback);
	}
	else
	{
		switch ((int)sc->m_cErrorReason)
		{
		case MsgLoginSC::ER_PASSWORD_ERROR:
			m_HUDLogin->SetTip(TEXT("Password Error"));
			break;
		case MsgLoginSC::ER_HAS_LOGIN_ERROR:
			m_HUDLogin->SetTip(TEXT("This Role Has Login"));
			break;
		case MsgLoginSC::ER_RELOGIN_ERROR:
			m_HUDLogin->SetTip(TEXT("Last login error,please login again!!"));
			break;
		}
		m_HUDLogin->SetLoadingShow(false);
	}
}

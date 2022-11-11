// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../MsgCommon.h"

#pragma pack(push,4)
// 152-role login seq
struct MsgLoginCS
{
	MsgLoginCS()
	{
		memset((void*)m_strRoleName, 0, sizeof(m_strRoleName));
		memset((void*)m_strPassword, 0, sizeof(m_strPassword));
	}
	int8 m_strRoleName[NSTcpClient::g_nRoleNameSize];
	int8 m_strPassword[NSTcpClient::g_nRolePasswordSize];
};

// 153-role login rep
struct MsgLoginSC
{
	enum EnLoginStatus
	{
		LS_DEFAULT = -1,

		LS_LOGIN_OK,
		LS_LOGIN_ERROR,
	};

	enum EnErrorReason
	{
		ER_DEFAULT = -1,

		ER_NO_ERROR,
		ER_PASSWORD_ERROR,
		ER_HAS_LOGIN_ERROR,
		ER_RELOGIN_ERROR,
	};

	MsgLoginSC()
	{
		memset((void*)m_strRoleName, 0, sizeof(m_strRoleName));
		memset((void*)m_strPassword, 0, sizeof(m_strPassword));
		m_cLoginStatus = LS_DEFAULT;
		m_cErrorReason = ER_DEFAULT;
		memset((void*)m_nReserve, 0, sizeof(m_nReserve));
	}

	int8 m_strRoleName[NSTcpClient::g_nRoleNameSize];
	int8 m_strPassword[NSTcpClient::g_nRolePasswordSize];
	int8 m_cLoginStatus;
	int8 m_cErrorReason;
	int8 m_nReserve[2];
};
#pragma pack(pop)


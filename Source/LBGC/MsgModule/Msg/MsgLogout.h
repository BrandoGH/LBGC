// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../MsgCommon.h"

#pragma pack(push,4)
// 159 MSG_TYPE_ROLE_MODEL_REMOVE
struct MsgLogoutSC
{
	MsgLogoutSC()
	{
		memset(m_strRoleName, 0, sizeof(m_strRoleName));
	}

	int8 m_strRoleName[NSTcpClient::g_nRoleNameSize];
};
#pragma pack(pop)

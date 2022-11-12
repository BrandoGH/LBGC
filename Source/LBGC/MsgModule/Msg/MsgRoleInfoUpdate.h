// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../MsgCommon.h"


#pragma pack(push,4)

// 157 MSG_TYPE_ROLE_INFO_UPDATE_CS
struct MsgRoleInfoUpdateCS
{
	MsgRoleInfoUpdateCS()
	{
		m_roleX = 0.0;
		m_roleY = 0.0;
		m_roleZ = 0.0;
	}

	MsgDouble m_roleX;
	MsgDouble m_roleY;
	MsgDouble m_roleZ;
};

// 158 MSG_TYPE_ROLE_INFO_UPDATE_SC
struct MsgRoleInfoUpdateSC
{
	MsgRoleInfoUpdateSC()
	{
		memset(m_targetRoleName, 0, sizeof(m_targetRoleName));
		m_roleX = 0.0;
		m_roleY = 0.0;
		m_roleZ = 0.0;
	}

	int8 m_targetRoleName[NSTcpClient::g_nRoleNameSize];
	MsgDouble m_roleX;
	MsgDouble m_roleY;
	MsgDouble m_roleZ;
};

#pragma pack(pop)

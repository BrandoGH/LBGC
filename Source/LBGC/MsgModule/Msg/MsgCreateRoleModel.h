// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../MsgCommon.h"

#pragma pack(push,4)
// msg type: 155
struct MsgCreateRoleCS
{
	MsgCreateRoleCS()
	{
		memset(m_nReserve, 0, sizeof(m_nReserve));
	}
	int8 m_nReserve[4];
};

// msg type: 156
struct MsgCreateRoleSC
{
	MsgCreateRoleSC()
	{
		memset(m_strCreateRoleName, 0, sizeof(m_strCreateRoleName));
		m_roleX = 0.0;
		m_roleY = 0.0;
		m_roleZ = 0.0;
	}
	int8 m_strCreateRoleName[NSTcpClient::g_nRoleNameSize];
	MsgDouble m_roleX;
	MsgDouble m_roleY;
	MsgDouble m_roleZ;
};

#pragma pack(pop)

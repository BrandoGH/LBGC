// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../MsgCommon.h"


#pragma pack(push,4)

// 157 MSG_TYPE_ROLE_INFO_UPDATE_CS
struct MsgRoleInfoUpdateCS
{
	enum EnJumpFlag
	{
		EJF_NORMAL,
		EJF_JUMPING,
	};

	MsgRoleInfoUpdateCS()
	{
		m_roleX = 0.0;
		m_roleY = 0.0;
		m_roleZ = 0.0;
		m_jumpFlag = EJF_NORMAL;
		m_velocity = 0;
		m_reserve = 0;
	}

	MsgDouble m_roleX;
	MsgDouble m_roleY;
	MsgDouble m_roleZ;
	int8 m_jumpFlag;
	int8 m_reserve;
	uint16 m_velocity; // max speed
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
		m_jumpFlag = MsgRoleInfoUpdateCS::EJF_NORMAL;
		m_velocity = 0;
		m_reserve = 0;
	}

	int8 m_targetRoleName[NSTcpClient::g_nRoleNameSize];
	MsgDouble m_roleX;
	MsgDouble m_roleY;
	MsgDouble m_roleZ;
	char m_jumpFlag;
	char m_reserve;
	uint16 m_velocity; // max speed
};

#pragma pack(pop)

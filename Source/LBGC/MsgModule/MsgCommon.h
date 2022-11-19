// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

namespace NSTcpClient
{
	const int g_nReadBufferMaxSize = 1024 * 50;
	const int g_nSendBufferMaxSize = 1024;
	const int g_nOnceMsgSize = 1024;
	const int g_nRoleNameSize = 32;
	const int g_nRolePasswordSize = 64;
}



#pragma pack(push,4)


struct MsgHeader
{
	MsgHeader()
	{
		reset();
	}

	void reset()
	{
		m_nMsgLen = 0;
		m_nMsgType = 0;
	}

	uint16 m_nMsgLen;
	uint16 m_nMsgType;

	// server use, client no need
	uint16 m_nSender;
	uint16 m_nReceiver;
	uint8 m_nProxyer;
	uint8 m_nReserve[3];
	uint64 m_nClientSrcSeq;
};

struct MsgEnder
{
	MsgEnder()
	{
		reset();
	}

	void reset()
	{
		memset(m_bytesMD5, 0, sizeof(m_bytesMD5));
	}

	uint8 m_bytesMD5[16];
};

// Float number type(data communication)
union MsgFloat
{
	MsgFloat()
	{
		reset();
	}
	MsgFloat(float inFloat)
	{
		m_float = inFloat;
	}
	MsgFloat& operator=(float inFloat)
	{
		m_float = inFloat;
		return *this;
	}
	void reset()
	{
		m_float = 0.f;
	}

	float m_float;
	uint8 m_bytes[sizeof(float)];
};
union MsgDouble
{
	MsgDouble()
	{
		reset();
	}
	MsgDouble(double inDouble)
	{
		m_double = inDouble;
	}
	MsgDouble& operator=(double inDouble)
	{
		m_double = inDouble;
		return *this;
	}
	void reset()
	{
		m_double = 0.0;
	}

	double m_double;
	uint8 m_bytes[sizeof(double)];
};

/*
	150-65535	client communication type
*/
enum EnMsgType
{
	// heart
	MSG_TYPE_CS_START					= 150,

	MSG_TYPE_HEART_CS					= MSG_TYPE_CS_START,
	MSG_TYPE_HEART_SC					= 151,
	MSG_TYPE_LOGIN_CS					= 152,
	MSG_TYPE_LOGIN_SC					= 153,
	MSG_TYPE_CREATE_ROLE_CS				= 155,
	MSG_TYPE_CREATE_ROLE_SC				= 156,
	MSG_TYPE_ROLE_INFO_UPDATE_CS		= 157,
	MSG_TYPE_ROLE_INFO_UPDATE_SC		= 158,
	MSG_TYPE_ROLE_MODEL_REMOVE			= 159,

	MSG_CODE_MAX,
};



#pragma pack(pop)
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#define I_MSG_HEART_CS "\x4C\x42\x47\x53"
#define I_MSG_HEART_SC "\x53\x47\x42\x4C"

#pragma pack(push,4)

struct MsgHeartCS	// 150-client heart req
{
	MsgHeartCS()
	{
		memset(m_bytesHeart, 0, sizeof(m_bytesHeart));
	}
	uint8 m_bytesHeart[4];		// LBGS = 0x4C 0x42 0x47 0x53
};

struct MsgHeartSC	// 151-client heart rep
{
	MsgHeartSC()
	{
		memset(m_bytesHeart, 0, sizeof(m_bytesHeart));
	}
	uint8 m_bytesHeart[4];		// SGBL = 0x53 0x47 0x42 0x4C
};

#pragma pack(pop)

// Fill out your copyright notice in the Description page of Project Settings.

#ifdef LBGS_DEBUG
PRAGMA_DISABLE_OPTIMIZATION
#endif // LBGS_DEBUG
#include "AyncTaskSender.h"
#include "../GameInstance/LBGCGameInstance.h"
#include "TcpClient.h"
#include "../MsgModule/MsgCommon.h"
#include "../MsgModule/Msg/MsgHeart.h"
#include "../MsgModule/CommonTool.h"
#include <Sockets.h>

AyncTaskSender::AyncTaskSender(int32 HeartSec)
	: m_sendSize(0)
	, m_sleepSum(0)
	, m_heartSec(HeartSec)
{
}

void AyncTaskSender::DoWork()
{
	UTcpClient* m_client = LBGC_INSTANCE->GetTcpClient();

	if (!m_client ||
		!m_client->GetSocket() ||
		!m_client->IsConnect())
	{
		return;
	}

	while (!m_client->IsWillDestroy())
	{
		if (m_sleepSum < m_heartSec)
		{
			FWindowsPlatformProcess::Sleep(1);
			++m_sleepSum;
			continue;
		}
		m_sleepSum = 0;

		SendHeartInfo();
		UE_LOG(LogTemp, Warning, TEXT("send heart"));


		FWindowsPlatformProcess::Sleep(1);
		++m_sleepSum;
	}
}

void AyncTaskSender::SendHeartInfo()
{
	int32 byteSend = 0;

	MsgHeader header;
	header.m_nMsgLen = sizeof(MsgHeader) + sizeof(MsgHeartCS) + sizeof(MsgEnder);
	header.m_nMsgType = MSG_TYPE_HEART_CS;

	TArray<uint8> sendData;
	sendData.Init(0, header.m_nMsgLen);

	MsgHeartCS cs;
	memmove(cs.m_bytesHeart, I_MSG_HEART_CS, sizeof(MsgHeartCS));

	MsgEnder ender;
	memmove(sendData.GetData(), (const char*)&header, sizeof(MsgHeader));
	memmove(sendData.GetData() + sizeof(MsgHeader), (const char*)&cs, sizeof(MsgHeartCS));
	CommonTool::HashMd5Data(sendData.GetData(), sizeof(MsgHeader) + sizeof(MsgHeartCS), ender.m_bytesMD5);
	memmove(sendData.GetData() + sizeof(MsgHeader) + sizeof(MsgHeartCS), (const uint8*)&ender, sizeof(MsgEnder));

	UTcpClient* tcpClient = LBGC_INSTANCE->GetTcpClient();
	if (tcpClient && tcpClient->GetSocket())
	{
		tcpClient->GetSocket()->Send(sendData.GetData(), sendData.Num(), byteSend);
	}
}

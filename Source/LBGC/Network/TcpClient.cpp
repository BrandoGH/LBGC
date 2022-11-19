// Fill out your copyright notice in the Description page of Project Settings.

#ifdef LBGS_DEBUG
PRAGMA_DISABLE_OPTIMIZATION
#endif // LBGS_DEBUG
#include "TcpClient.h"
#include "Sockets.h"
#include "Networking/Public/Common/TcpSocketBuilder.h"
#include "../MsgModule/MsgCommon.h"
#include "AyncTackConnector.h"
#include "AyncTaskReader.h"
#include "AyncTaskSender.h"
#include "../GameInstance/LBGCGameInstance.h"
#include "../MsgModule/CommonTool.h"
#include "../MsgModule/Msg/MsgCreateRoleModel.h"
#include "../ConfigModule/StartupConfig.h"

UTcpClient::UTcpClient()
	: m_pSocketClient(NULL)
	, m_bConnected(false)
	, m_bWillDestroy(false)
	, m_bTcpTimeout(false)
{
	m_dgReader.BindUObject(this, &UTcpClient::OnMsgRead);
}

UTcpClient::~UTcpClient()
{
	m_bWillDestroy = true;
	// wait other thread op done (send/read)
	FWindowsPlatformProcess::Sleep(1.F);
	CloseSocket();
#ifdef LBGS_DEBUG
	UE_LOG(LogTemp, Warning, TEXT("Socket has close!"));
#endif // LBGS_DEBUG
}

void UTcpClient::Init(int ClientSeq)
{
	if (m_pSocketClient)
	{
		return;
	}
	FString desc = FString::Printf(TEXT("ClientSocket_%d"), ClientSeq);
	m_pSocketClient = FTcpSocketBuilder(desc)
		.AsReusable()
		.AsBlocking()
		.WithReceiveBufferSize(NSTcpClient::g_nReadBufferMaxSize)
		.WithSendBufferSize(NSTcpClient::g_nSendBufferMaxSize);
}

void UTcpClient::Connect(const FString& IP, uint16 Port, const FClientConnectDelegate& DGConnected)
{
	FAsyncTask<AyncTaskConnector>* task = new FAsyncTask<AyncTaskConnector>(IP, Port, DGConnected);
	task->StartBackgroundTask();
}

void UTcpClient::CloseSocket()
{
	if (m_pSocketClient)
	{
		m_pSocketClient->Close();
	}
}

void UTcpClient::ShutdownRead()
{
	if (m_pSocketClient)
	{
		m_pSocketClient->Shutdown(ESocketShutdownMode::Read);
	}
}

void UTcpClient::ShutdownWrite()
{
	if (m_pSocketClient)
	{
		m_pSocketClient->Shutdown(ESocketShutdownMode::Write);
	}
}

void UTcpClient::Send(const uint8* Data, int32 Count, int32 MsgType, const ExpectMsgStruct& Expect)
{
	int32 byteSend = 0;
	if (!GetSocket())
	{
		return;
	}

	m_mapExpectMsgtypeToDelegate.Add(Expect.ExpectMsgType, Expect.ExpectDg);

	MsgHeader header;
	header.m_nMsgLen = sizeof(MsgHeader) + Count + sizeof(MsgEnder);
	header.m_nMsgType = MsgType;

	TArray<uint8> sendData;
	sendData.Init(0, header.m_nMsgLen);

	MsgEnder ender;
	memmove(sendData.GetData(), (const char*)&header, sizeof(MsgHeader));
	memmove(sendData.GetData() + sizeof(MsgHeader), Data, Count);
	CommonTool::HashMd5Data(sendData.GetData(), sizeof(MsgHeader) + Count, ender.m_bytesMD5);
	memmove(sendData.GetData() + sizeof(MsgHeader) + Count, (const uint8*)&ender, sizeof(MsgEnder));
	GetSocket()->Send(sendData.GetData(), sendData.Num(), byteSend);
}

void UTcpClient::StartRead()
{
	FAsyncTask<AyncTaskReader>* TaskReader = new FAsyncTask<AyncTaskReader>(m_dgReader);
	TaskReader->StartBackgroundTask();
}

void UTcpClient::StartSendHeart()
{
	UStartupConfig* config = LBGC_INSTANCE->GetStartupConfig();
	if (!config)
	{
		return;
	}

	FAsyncTask<AyncTaskSender>* TaskSender = new FAsyncTask<AyncTaskSender>(config->GetCfgHeartSec());
	TaskSender->StartBackgroundTask();
}

void UTcpClient::OnMsgRead(const TArray<uint8>& msg)
{
	if (!LBGC_INSTANCE)
	{
		return;
	}

	UStartupConfig* config = LBGC_INSTANCE->GetStartupConfig();
	if (!config)
	{
		return;
	}

	LBGC_INSTANCE->GetTimerManager().ClearTimer(m_timeoutHandle);
	FTimerDelegate dgTimeout;
	dgTimeout.BindLambda(
		[&]()
		{
			m_bTcpTimeout = true;
			CloseSocket();
		}
	);
	LBGC_INSTANCE->GetTimerManager().SetTimer(m_timeoutHandle, dgTimeout, config->GetCfgHeartSec() * 2, false);


	MsgHeader* header = (MsgHeader*)msg.GetData();
	if (!header)
	{
		return;
	}

	if (m_mapExpectMsgtypeToDelegate.Contains(header->m_nMsgType))
	{
		m_mapExpectMsgtypeToDelegate[header->m_nMsgType].ExecuteIfBound(msg.GetData() + sizeof(MsgHeader));
	}
}

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

void UTcpClient::StartRead()
{
	FAsyncTask<AyncTaskReader>* TaskReader = new FAsyncTask<AyncTaskReader>(m_dgReader);
	TaskReader->StartBackgroundTask();
}

void UTcpClient::StartSendHeart()
{
	FAsyncTask<AyncTaskSender>* TaskSender = new FAsyncTask<AyncTaskSender>(30);
	TaskSender->StartBackgroundTask();
}

void UTcpClient::OnMsgRead(const TArray<uint8>& msg)
{
	if (!LBGC_INSTANCE)
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
	LBGC_INSTANCE->GetTimerManager().SetTimer(m_timeoutHandle, dgTimeout, 60, false);
}

// Fill out your copyright notice in the Description page of Project Settings.

#ifdef LBGS_DEBUG
PRAGMA_DISABLE_OPTIMIZATION
#endif // LBGS_DEBUG
#include "AyncTackConnector.h"
#include "../GameInstance/LBGCGameInstance.h"
#include <Networking/Public/Interfaces/IPv4/IPv4Endpoint.h>
#include <SocketSubsystem.h>
#include "TcpClient.h"
#include <Sockets.h>

AyncTaskConnector::AyncTaskConnector(const FString& IP, uint16 Port, const FClientConnectDelegate& DGConnected)
	: m_ip(IP)
	, m_port(Port)
	, m_dgConnected(DGConnected)
{
}

AyncTaskConnector::~AyncTaskConnector()
{
}

void AyncTaskConnector::DoWork()
{
	if (!LBGC_INSTANCE)
	{
		return;
	}
	UTcpClient* tcpClient = LBGC_INSTANCE->GetTcpClient();
	if (!tcpClient || !tcpClient->GetSocket())
	{
		return;
	}

	
	FIPv4Endpoint ServerEndpoint;
	FIPv4Endpoint::Parse(m_ip, ServerEndpoint);
	TSharedPtr<FInternetAddr> addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	bool Vlalid = false;
	addr->SetIp(*m_ip, Vlalid);
	addr->SetPort(m_port);

	bool ok = Vlalid && tcpClient->GetSocket()->Connect(*addr);
	if (ok)
	{
		AsyncTask(ENamedThreads::GameThread,
			[&]()
			{
				m_dgConnected.ExecuteIfBound(true, FString(TEXT("Connect Success!")));
			});
		return;
	}
	AsyncTask(ENamedThreads::GameThread,
		[&]()
		{
			m_dgConnected.ExecuteIfBound(false, FString(TEXT("Connect Fail")));
		});
}

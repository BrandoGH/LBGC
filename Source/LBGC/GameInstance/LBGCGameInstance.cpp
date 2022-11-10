// Fill out your copyright notice in the Description page of Project Settings.

#ifdef LBGS_DEBUG
PRAGMA_DISABLE_OPTIMIZATION
#endif // LBGS_DEBUG
#include "LBGCGameInstance.h"
#include "../Network/TcpClient.h"

void ULBGCGameInstance::Init()
{
	m_tcpClient = NULL;
}

void ULBGCGameInstance::Shutdown()
{
	DeleteTcpClient();
}

UTcpClient* ULBGCGameInstance::GetTcpClient(int ClientSeq)
{
	if (m_tcpClient)
	{
		return m_tcpClient;
	}

	m_tcpClient = NewObject<UTcpClient>();
	m_tcpClient->AddToRoot();
	m_tcpClient->Init(ClientSeq);
	return m_tcpClient;
}

ULBGCGameInstance* ULBGCGameInstance::instance = NULL;
ULBGCGameInstance* ULBGCGameInstance::GetInstance()
{
	if (GEngine)
	{
		FWorldContext* context = GEngine->GetWorldContextFromGameViewport(GEngine->GameViewport);
		if (context)
		{
			instance = Cast<ULBGCGameInstance>(context->OwningGameInstance);
		}
	}

	return instance;
}

void ULBGCGameInstance::DeleteTcpClient()
{
	if (m_tcpClient)
	{
		m_tcpClient->RemoveFromRoot();
		m_tcpClient = NULL;
	}
}

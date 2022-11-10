// Fill out your copyright notice in the Description page of Project Settings.

#ifdef LBGS_DEBUG
PRAGMA_DISABLE_OPTIMIZATION
#endif // LBGS_DEBUG
#include "LBGCGameInstance.h"
#include "../Network/TcpClient.h"
#include <UObject/UObjectGlobals.h>
#include <Kismet/GameplayStatics.h>

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

void ULBGCGameInstance::LoadMainGameLevel(const FAyncLoadLevelDelegate& callback)
{
	AyncLoadLevel(TEXT("/Game/LGGC_Game/Maps/"), TEXT("Map_Test"), callback);
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

void ULBGCGameInstance::AyncLoadLevel(const FString& mapDir, const FString& mapName, const FAyncLoadLevelDelegate& dg)
{
	LoadPackageAsync(mapDir + mapName,
		FLoadPackageAsyncDelegate::CreateLambda([=](const FName& PackageName, UPackage* LoadedPackage, EAsyncLoadingResult::Type Result)
			{
				if (Result == EAsyncLoadingResult::Succeeded)
				{
					UGameplayStatics::OpenLevel(this, FName(*mapName));
					dg.ExecuteIfBound(true);
				}
				else
				{
					dg.ExecuteIfBound(false);
				}
			}), 0, PKG_ContainsMap);
}

// Fill out your copyright notice in the Description page of Project Settings.

#ifdef LBGS_DEBUG
PRAGMA_DISABLE_OPTIMIZATION
#endif // LBGS_DEBUG
#include "LBGCGameInstance.h"
#include "../Network/TcpClient.h"
#include <UObject/UObjectGlobals.h>
#include <Kismet/GameplayStatics.h>
#include "Engine/Engine.h"

namespace
{
const FVector g_vecDefaultCreateMainRoleLoc = FVector(0.f, 0.f, 190.f);
const FVector g_vecDefaultCreateMinorRoleLoc = FVector(0.f, 0.f, 88.f);
}

void ULBGCGameInstance::Init()
{
	m_tcpClient = NULL;
	SpawnLocalRole = LoadClass<AMainRole>(NULL, TEXT("Blueprint'/Game/LGGC_Game/Blueprint/Role/BP_MainRole.BP_MainRole_C'"));
	SpawnRemoteRole = LoadClass<AMinorRole>(NULL, TEXT("BBlueprint'/Game/LGGC_Game/Blueprint/Role/BP_MinorRole.BP_MinorRole_C'"));
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

void ULBGCGameInstance::PrintDebugMessageOnScreen(int32 Key, float TimeToDisplay, FColor DisplayColor, const FString& DebugMessage, bool bNewerOnTop, const FVector2D& TextScale)
{
#ifdef LBGS_DEBUG
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(Key, TimeToDisplay, DisplayColor, DebugMessage, bNewerOnTop, TextScale);
	}
#endif // LBGS_DEBUG
}

ULBGCGameInstance* ULBGCGameInstance::instance = NULL;
void ULBGCGameInstance::CreateLocalRole()
{
	if (GetWorld())
	{
		GetWorld()->SpawnActor<AMainRole>(SpawnLocalRole, g_vecDefaultCreateMainRoleLoc, FRotator(0.f));
	}
}
void ULBGCGameInstance::CreateRemoteRole(const FString& roleName)
{
	m_mapRoleNameToMinorRoleModel.Emplace(roleName, SpawnRemoteRole);
	if (GetWorld())
	{
		GetWorld()->SpawnActor<AMinorRole>(m_mapRoleNameToMinorRoleModel[roleName], g_vecDefaultCreateMinorRoleLoc, FRotator(0.f));
	}
}
AMainRole* ULBGCGameInstance::GetLocalRole()
{
	return Cast<AMainRole>(SpawnLocalRole->GetDefaultObject());
}
AMinorRole* ULBGCGameInstance::GetMinorRole(const FString& roleName)
{
	if (m_mapRoleNameToMinorRoleModel.Contains(roleName))
	{
		return Cast<AMinorRole>(m_mapRoleNameToMinorRoleModel[roleName]->GetDefaultObject());
	}
	return NULL;
}
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

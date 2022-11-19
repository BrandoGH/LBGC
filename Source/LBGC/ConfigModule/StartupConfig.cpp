// Fill out your copyright notice in the Description page of Project Settings.


#include "StartupConfig.h"

namespace
{
const FString g_strConfigFileName = "server.xml";
const FString g_strLoadNodeLevel = "config.gateserver.data";
}

UStartupConfig::UStartupConfig()
	: m_bLoadSuccess(false)
{
	if (LoadXml())
	{
		InitConfig();
	}
}

bool UStartupConfig::LoadXml()
{
	if (m_bLoadSuccess)
	{
		return m_bLoadSuccess;
	}

	FString path = LBGS_CONFIG_DIR + g_strConfigFileName;
	m_xml = MakeShared<FXmlFile>();
	if (m_xml->LoadFile(path, EConstructMethod::ConstructFromFile))
	{
		m_bLoadSuccess = true;
	}

	return m_bLoadSuccess;
}

void UStartupConfig::InitConfig()
{
	TArray<FString> LevelArr;
	g_strLoadNodeLevel.ParseIntoArray(LevelArr, TEXT("."), true);
	FXmlNode* Node = m_xml->GetRootNode();
	for (int i = 1; i < LevelArr.Num(); ++i)
	{
		if (Node)
		{
			Node = Node->FindChildNode(LevelArr[i]);
		}
	}

	m_cfg.m_ip = Node->FindChildNode(TEXT("ip"))->GetContent();
	m_cfg.m_port = FCString::Atoi(*Node->FindChildNode(TEXT("port"))->GetContent());
	m_cfg.m_heartSec = FCString::Atoi(*Node->FindChildNode(TEXT("heartbeat_interval_s"))->GetContent());
}

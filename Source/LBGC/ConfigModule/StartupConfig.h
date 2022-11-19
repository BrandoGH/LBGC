// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <XmlParser/Public/XmlFile.h>
#include "StartupConfig.generated.h"


/**
 * Startup Config
 */

struct ServerCfg
{
	ServerCfg()
	{
		m_ip = "";
		m_port = 0;
		m_heartSec = 0;
	}
	FString m_ip;
	uint16 m_port;
	uint32 m_heartSec;
};

UCLASS()
class LBGC_API UStartupConfig : public UObject
{
	GENERATED_BODY()

public:
	UStartupConfig();

	const FString GetCfgIp() { return m_cfg.m_ip; };
	uint16 GetCfgPort() { return m_cfg.m_port; };
	uint32 GetCfgHeartSec() { return m_cfg.m_heartSec; };

private:
	bool LoadXml();
	void InitConfig();

private:
	TSharedPtr<FXmlFile> m_xml;
	bool m_bLoadSuccess;
	ServerCfg m_cfg;
};

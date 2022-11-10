// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <Async/AsyncWork.h>
#include "TcpClient.h"

/**
 * 
 */
class AyncTaskConnector : public FNonAbandonableTask
{
	friend class FAutoDeleteAsyncTask<AyncTaskConnector>;
public:
	AyncTaskConnector(
		const FString& IP,
		uint16 Port,
		const FClientConnectDelegate& DGConnected);
	~AyncTaskConnector();

	void DoWork();

	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(AyncTaskConnector, STATGROUP_ThreadPoolAsyncTasks);
	}

private:
	FString m_ip;
	uint16 m_port;
	FClientConnectDelegate m_dgConnected;

};

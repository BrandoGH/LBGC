// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class AyncTaskSender : public FNonAbandonableTask
{
	friend class FAutoDeleteAsyncTask<AyncTaskSender>;

public:
	AyncTaskSender(int32 HeartSec);

	void DoWork();
	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(AyncTaskSender, STATGROUP_ThreadPoolAsyncTasks);
	}

private:
	void SendHeartInfo();

private:
	int32 m_sendSize;

	int32 m_heartSec;
	// sumulate sleep xxx sec, close game ,this thread quick exit
	int32 m_sleepSum;
};

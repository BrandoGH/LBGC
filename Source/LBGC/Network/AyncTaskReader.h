// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Async/AsyncWork.h>
#include "TcpClient.h"
#include "../MsgModule/MsgCommon.h"

/**
 * 
 */
class AyncTaskReader : public FNonAbandonableTask
{
public:
	friend class FAutoDeleteAsyncTask<AyncTaskReader>;
public:
	AyncTaskReader(const FMsgReaderDelegate& ReadCallback);
	void DoWork();
	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(AyncTaskReader, STATGROUP_ThreadPoolAsyncTasks);
	}

private:
	TArray<uint8> m_arrReadMsg;
	TArray<uint8> m_onceMsg;
	FMsgReaderDelegate m_dgReadCallback;
	TQueue<TArray<uint8>> m_queueMsg;

	MsgHeader m_msgHeader;
	MsgEnder m_msgEnder;
	int32 m_nReadSise;
	int32 m_nHasReadDataSize;
	int32 m_nNextNeedReadSize;
	int32 m_nLastHasReadSize;
	bool m_bHeaderIntegrated;
};

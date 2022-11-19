// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <Engine/EngineTypes.h>
#include "TcpClient.generated.h"

DECLARE_DELEGATE_TwoParams(FClientConnectDelegate, bool /*ok*/, const FString& /*info*/);
DECLARE_DELEGATE_OneParam(FMsgReaderDelegate, const TArray<uint8>& /*msg*/);
DECLARE_DELEGATE_OneParam(FMsgCallbackToExpectMsg, const uint8* /*msg*/);

/**
 * 
 */
UCLASS()
class LBGC_API UTcpClient : public UObject
{
	GENERATED_BODY()
	
public:
	struct ExpectMsgStruct
	{
		int32 ExpectMsgType = 0;
		FMsgCallbackToExpectMsg ExpectDg;
	};

public:

	UTcpClient();
	~UTcpClient();

	void Init(int ClientSeq = 0);
	void Connect(const FString& IP, uint16 Port, const FClientConnectDelegate& DGConnected);
	void CloseSocket();
	void ShutdownRead();
	void ShutdownWrite();
	class FSocket* GetSocket() { return m_pSocketClient; }
	void SetConnectedFlag(bool bConnected) { m_bConnected = bConnected; }
	bool IsConnect() { return m_bConnected; }
	bool IsWillDestroy() { return m_bWillDestroy; };
	bool IsTimerout() { return m_bTcpTimeout; }

	void Send(const uint8* Data, int32 Count, int32 MsgType, const ExpectMsgStruct& Expect);
	void RegisterMsgCallback(const ExpectMsgStruct& Expect);

	void StartRead();
	void StartSendHeart();

private:
	void OnMsgRead(const TArray<uint8>& msg);

private:
	class FSocket* m_pSocketClient;
	bool m_bConnected;
	bool m_bWillDestroy;
	FMsgReaderDelegate m_dgReader;
	TMap<int32, FMsgCallbackToExpectMsg> m_mapExpectMsgtypeToDelegate;

	bool m_bTcpTimeout;
	FTimerHandle m_timeoutHandle;
};

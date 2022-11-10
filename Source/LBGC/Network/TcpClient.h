// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TcpClient.generated.h"

DECLARE_DELEGATE_TwoParams(FClientConnectDelegate, bool /*ok*/, const FString& /*info*/);
/**
 * 
 */
UCLASS()
class LBGC_API UTcpClient : public UObject
{
	GENERATED_BODY()
	
public:

	UTcpClient();
	~UTcpClient();

	void Init(int ClientSeq = 0);
	void Connect(const FString& IP, uint16 Port, const FClientConnectDelegate& DGConnected);
	void CloseSocket();
	void ShutdownRead();
	void ShutdownWrite();
	class FSocket* GetSocket() { return m_pSocketClient; }

private:
	class FSocket* m_pSocketClient;
};

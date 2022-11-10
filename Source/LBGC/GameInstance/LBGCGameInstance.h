// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "LBGCGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class LBGC_API ULBGCGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void Shutdown() override;

	class UTcpClient* GetTcpClient(int ClientSeq = 0);

public:
	UFUNCTION(BlueprintCallable, Category = "ULBGCGameInstance Func")
		static ULBGCGameInstance* GetInstance();

private:
	void DeleteTcpClient();

private:
	static ULBGCGameInstance* instance;

	class UTcpClient* m_tcpClient;
	
};

#define LBGC_INSTANCE ULBGCGameInstance::GetInstance()

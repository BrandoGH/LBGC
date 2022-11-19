// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "../Network/TcpClient.h"
#include "MainPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class LBGC_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AMainPlayerController();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	void OnRoleInfoUpdateSC(const uint8* msg);

private:
	void RegisterLogoutMsg();
	void SendCreateRoleModel();
	void OnMsgCreateRoleSC(const uint8* msg);

	void SyncMinorRoleInfo(float DeltaTime);

private:
	FMsgCallbackToExpectMsg m_dgMsgCreateRoleSC;

	class AMinorRole* m_minorRole = NULL;
	FVector m_vecTarget;

	FRotator m_lastRotInterp;
	FVector m_lastVecInterp;
};

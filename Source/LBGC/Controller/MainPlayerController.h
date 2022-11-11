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

private:
	void SendCreateRoleModel();
	void OnMsgCreateRoleSC(const uint8* msg);

private:
	FMsgCallbackToExpectMsg m_dgMsgCreateRoleSC;
};

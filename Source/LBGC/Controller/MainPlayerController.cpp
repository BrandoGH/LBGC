// Fill out your copyright notice in the Description page of Project Settings.

#ifdef LBGS_DEBUG
PRAGMA_DISABLE_OPTIMIZATION
#endif // LBGS_DEBUG
#include "MainPlayerController.h"
#include "../GameInstance/LBGCGameInstance.h"
#include "../MsgModule/Msg/MsgCreateRoleModel.h"
#include <Kismet/GameplayStatics.h>

AMainPlayerController::AMainPlayerController()
{
}

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();
	SendCreateRoleModel();
}

void AMainPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMainPlayerController::SendCreateRoleModel()
{
	if (!LBGC_INSTANCE || !LBGC_INSTANCE->GetTcpClient())
	{
		return;
	}
	MsgCreateRoleCS cs;
	m_dgMsgCreateRoleSC.BindUObject(this, &AMainPlayerController::OnMsgCreateRoleSC);
	UTcpClient::ExpectMsgStruct expect;
	expect.ExpectMsgType = MSG_TYPE_CREATE_ROLE_SC;
	expect.ExpectDg = m_dgMsgCreateRoleSC;
	LBGC_INSTANCE->GetTcpClient()->Send((const uint8*)&cs, sizeof(MsgCreateRoleCS), MSG_TYPE_CREATE_ROLE_CS, expect);
}

void AMainPlayerController::OnMsgCreateRoleSC(const uint8* msg)
{
	if (!LBGC_INSTANCE)
	{
		return;
	}

	MsgCreateRoleSC* sc = (MsgCreateRoleSC*)msg;
	if (!sc)
	{
		return;
	}

	FString localRoleName = LBGC_INSTANCE->GetLocalRoleName();
	FString willCreateRoleName = FString(strlen((const char*)sc->m_strCreateRoleName), (const char*)sc->m_strCreateRoleName);

	if (localRoleName == willCreateRoleName)
	{
		if (LBGC_INSTANCE->IsCreateModel())
		{
			return;
		}
		LBGC_INSTANCE->CreateLocalRole();
		LBGC_INSTANCE->SetCreateModelFlag(true);
		return;
	}
}

// Fill out your copyright notice in the Description page of Project Settings.

#ifdef LBGS_DEBUG
PRAGMA_DISABLE_OPTIMIZATION
#endif // LBGS_DEBUG
#include "MainPlayerController.h"
#include "../GameInstance/LBGCGameInstance.h"
#include "../MsgModule/Msg/MsgCreateRoleModel.h"
#include <Kismet/GameplayStatics.h>
#include "../MsgModule/Msg/MsgRoleInfoUpdate.h"
#include <AIController.h>
#include <GameFramework/Character.h>
#include <GameFramework/PawnMovementComponent.h>
#include <Kismet/KismetMathLibrary.h>

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

	if (m_minorRole)
	{
		FVector checkLocation = m_minorRole->GetActorLocation();
		checkLocation.Z = 0.f;
		FVector checkLocation2 = m_vecTarget;
		checkLocation2.Z = 0.f;

		if (checkLocation != checkLocation2)
		{
			FRotator CurRot = m_minorRole->GetActorRotation();
			FRotator TargetRot = UKismetMathLibrary::FindLookAtRotation(m_minorRole->GetActorLocation(), m_vecTarget);
			TargetRot.Pitch = CurRot.Pitch;
			TargetRot.Roll = CurRot.Roll;

			m_lastRotInterp = FMath::RInterpTo(CurRot, TargetRot, DeltaTime, 3.f);
			m_minorRole->SetActorRotation(m_lastRotInterp);
		}

		m_lastVecInterp = FMath::VInterpTo(m_minorRole->GetActorLocation(), m_vecTarget, DeltaTime, 0.f);
		m_minorRole->SetActorLocation(m_lastVecInterp);
	}
	
}

void AMainPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	AMainRole* localRole = LBGC_INSTANCE->GetLocalRole();
	if (localRole)
	{
		localRole->Logout();
	}
}

void AMainPlayerController::OnRoleInfoUpdateSC(const uint8* msg)
{
	if (!LBGC_INSTANCE)
	{
		return;
	}
	MsgRoleInfoUpdateSC* sc = (MsgRoleInfoUpdateSC*)msg;
	if (!sc)
	{
		return;
	}

	FString scRoleName = FString(strlen((const char*)sc->m_targetRoleName), (const char*)sc->m_targetRoleName);
	LBGC_INSTANCE->PrintDebugMessageOnScreen(0, 1000.f, FColor::Yellow,
		FString::Printf(TEXT("alter [%s] location to X[%04lf] Y[%04lf] Z[%04lf]"),
			*scRoleName, sc->m_roleX.m_double, sc->m_roleY.m_double, sc->m_roleZ.m_double));

	m_minorRole = LBGC_INSTANCE->GetMinorRole(scRoleName);
	m_vecTarget = FVector(sc->m_roleX.m_double, sc->m_roleY.m_double, sc->m_roleZ.m_double);
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

	AMainRole* localRole = LBGC_INSTANCE->GetLocalRole();
	if (!localRole)
	{
		return;
	}

	MsgCreateRoleSC* sc = (MsgCreateRoleSC*)msg;
	if (!sc)
	{
		return;
	}

	FString localRoleName = localRole->GetRoleName();
	FString willCreateRoleName = FString(strlen((const char*)sc->m_strCreateRoleName), (const char*)sc->m_strCreateRoleName);

	if (localRoleName == willCreateRoleName)
	{
		if (localRole->IsCreateModel())
		{
			return;
		}

		LBGC_INSTANCE->CreateLocalRole();
		localRole->Login();
		LBGC_INSTANCE->PrintDebugMessageOnScreen(-1, 1000.f, FColor::Yellow, FString::Printf(TEXT("Has create myself: %s"), *localRoleName));
		return;
	}

	// create other role
	AMinorRole* minorRole = LBGC_INSTANCE->CreateRemoteRole(willCreateRoleName);
	if (minorRole)
	{
		minorRole->SetRoleName(willCreateRoleName);
		minorRole->Login();
	}

	LBGC_INSTANCE->PrintDebugMessageOnScreen(-1, 1000.f, FColor::Yellow, FString::Printf(TEXT("%s: shuold create other role[%s] on my clients"), *localRoleName, *willCreateRoleName));
}

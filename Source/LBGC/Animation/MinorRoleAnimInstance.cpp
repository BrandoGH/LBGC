// Fill out your copyright notice in the Description page of Project Settings.

#ifdef LBGS_DEBUG
PRAGMA_DISABLE_OPTIMIZATION
#endif // LBGS_DEBUG
#include "MinorRoleAnimInstance.h"
#include "../Character/MinorRole.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"
#include "../GameInstance/LBGCGameInstance.h"
#include <Components/CapsuleComponent.h>
#include <Engine/EngineTypes.h>
#include <Kismet/KismetSystemLibrary.h>

UMinorRoleAnimInstance::UMinorRoleAnimInstance()
	: UMainRoleAnimInstance()
{
}

void UMinorRoleAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UMinorRoleAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	UpdateMinorRoleAnimationProperties(DeltaSeconds);
}

void UMinorRoleAnimInstance::UpdateMinorRoleAnimationProperties(float fDeltaSeconds)
{
	if (!CheckMinorRole()) { return; }

	MoveSpeed = m_MinorRole->GetMoveVelocity();
	IsJumping = m_MinorRole->IsJumping();
}

bool UMinorRoleAnimInstance::TraceCheckFootIK(EnFootIkTraceType type)
{
	if (!CheckMinorRole() ||
		!m_MinorRole->GetMesh() ||
		!m_MinorRole->GetCapsuleComponent())
	{
		return false;
	}

	FVector VecStartLoc;
	FVector VecEndLoc;

	/*
		Trace range��
		Z : half of GetScaledCapsuleHalfHeight -> A GetCapsuleComponent length ray [knee to sole]
		X Y: foot socket location
	*/
	FVector vecFootSocket;
	if (type == FOOT_IK_L_TRACE)
	{
		vecFootSocket = m_MinorRole->GetMesh()->GetSocketLocation(FName("foot_lSocket"));
	}
	else
	{
		vecFootSocket = m_MinorRole->GetMesh()->GetSocketLocation(FName("foot_rSocket"));
	}

	float fTraceStartZ =
		m_MinorRole->GetCapsuleComponent()->GetRelativeLocation().Z -
		m_MinorRole->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() / 2.f;
	VecStartLoc.X = vecFootSocket.X;
	VecStartLoc.Y = vecFootSocket.Y;
	VecStartLoc.Z = fTraceStartZ;
	VecEndLoc.X = vecFootSocket.X;
	VecEndLoc.Y = vecFootSocket.Y;
	VecEndLoc.Z = VecStartLoc.Z - m_MinorRole->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	TArray<AActor*> ActorsToIgnore;
	TArray<TEnumAsByte<EObjectTypeQuery> > ObjectTypes;
	/*
	Default ObjectTypeQuery1 ！！ WorldStatic
	Default ObjectTypeQuery2 ！！ WorldDynamic
	Default ObjectTypeQuery3 ！！ Oawn
	Default ObjectTypeQuery4 ！！ PhysicasBody
	Default ObjectTypeQuery5 ！！ Vehicle
	Default ObjectTypeQuery6 ！！ Destructible
	*/
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery1);
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery2);

	FHitResult hitResult;
	bool bHit = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), VecStartLoc, VecEndLoc,
		ObjectTypes, false, ActorsToIgnore, EDrawDebugTrace::ForOneFrame, hitResult, true);

	if (!bHit)
	{
		m_fLFootGroundDistance = 0.f;
		m_fRFootGroundDistance = 0.f;
		return false;
	}

	// This distance has a negative sign
	if (type == FOOT_IK_L_TRACE)
	{
		m_fLFootGroundDistance = hitResult.Location.Z - m_MinorRole->GetMesh()->GetComponentLocation().Z;
	}
	else
	{
		m_fRFootGroundDistance = hitResult.Location.Z - m_MinorRole->GetMesh()->GetComponentLocation().Z;
	}

	return true;
}

bool UMinorRoleAnimInstance::CheckMinorRole()
{
	if (!m_MinorRole)
	{
		m_MinorRole = Cast<AMinorRole>(TryGetPawnOwner());
		if (!m_MinorRole)
		{
			return false;
		}
	}
	return true;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "MinorRoleAnimInstance.h"
#include "../Character/MinorRole.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"
#include "../GameInstance/LBGCGameInstance.h"

UMinorRoleAnimInstance::UMinorRoleAnimInstance()
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

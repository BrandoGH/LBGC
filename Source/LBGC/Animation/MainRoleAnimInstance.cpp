// Fill out your copyright notice in the Description page of Project Settings.


#include "MainRoleAnimInstance.h"
#include "../Character/MainRole.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"

UMainRoleAnimInstance::UMainRoleAnimInstance()
	: m_mainRole(NULL)
	, MoveSpeed(0.f)
	, IsJumping(false)
{
}

void UMainRoleAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UMainRoleAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	UpdateAnimationProperties(DeltaSeconds);
}

void UMainRoleAnimInstance::UpdateAnimationProperties(float fDeltaSeconds)
{
	if (!CheckMainRole()) { return; }

	FVector velocity = m_mainRole->GetVelocity();
	velocity.Z = 0;	// not deed jump z
	MoveSpeed = velocity.Size();
	if (m_mainRole->GetMovementComponent())
	{
		IsJumping = m_mainRole->GetMovementComponent()->IsFalling();
	}
}

bool UMainRoleAnimInstance::CheckMainRole()
{
	if (!m_mainRole)
	{
		m_mainRole =  Cast<AMainRole>(TryGetPawnOwner());
		if (!m_mainRole)
		{
			return false;
		}
	}
	return true;
}

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

	UpdateAnimationProperties(DeltaSeconds);
}

void UMinorRoleAnimInstance::UpdateAnimationProperties(float fDeltaSeconds)
{
	if (!CheckMinorRole()) { return; }

	FVector velocity = m_MinorRole->GetVelocity();
	velocity.Z = 0;	// not deed jump z
	MoveSpeed = velocity.Size();
	if (m_MinorRole->GetMovementComponent())
	{
		IsJumping = m_MinorRole->GetMovementComponent()->IsFalling();
	}

	if (LBGC_INSTANCE)
	{
		LBGC_INSTANCE->PrintDebugMessageOnScreen(0,10.f,FColor::Yellow, FString::Printf(TEXT("velocity[%f,%f]  IsJumping[%d]"),velocity.X, velocity.Y, IsJumping));
	}
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

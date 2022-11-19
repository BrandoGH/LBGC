// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MainRoleAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class LBGC_API UMainRoleAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	enum EnFootIkTraceType
	{
		FOOT_IK_L_TRACE,
		FOOT_IK_R_TRACE,
	};

public:
	UMainRoleAnimInstance();
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	UFUNCTION(BlueprintCallable, Category = "RoleAnimInstance Func")
		void UpdateMainRoleAnimationProperties(float fDeltaSeconds);

	UFUNCTION(BlueprintCallable, Category = "RoleAnimInstance Func")
		void UpdateFootIK(float fDeltaSeconds);

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RoleAnimInstance  Var")
		float MoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RoleAnimInstance  Var")
		bool IsJumping;

	// Foot IK
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RoleAnimInstance  Var")
		float LFootEffectorX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RoleAnimInstance  Var")
		float RFootEffectorX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RoleAnimInstance  Var")
		float PelvisZ;

protected:
	virtual bool TraceCheckFootIK(EnFootIkTraceType type);

private:
	bool CheckMainRole();

protected:
	// Foot IK
	// The distance from the left and right feet to the ground
	float m_fLFootGroundDistance;
	float m_fRFootGroundDistance;

private:
	class AMainRole* m_mainRole;

	
	
};

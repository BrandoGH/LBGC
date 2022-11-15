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
	UMainRoleAnimInstance();
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	UFUNCTION(BlueprintCallable, Category = "UMainRoleAnimInstance Func")
		void UpdateAnimationProperties(float fDeltaSeconds);

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMainRoleAnimInstance Var")
		float MoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMainRoleAnimInstance Var")
		bool IsJumping;

private:
	bool CheckMainRole();

private:
	class AMainRole* m_mainRole;
	
};

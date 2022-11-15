// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MinorRoleAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class LBGC_API UMinorRoleAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UMinorRoleAnimInstance();
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
	bool CheckMinorRole();

private:
	class AMinorRole* m_MinorRole;
};

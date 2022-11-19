// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MainRoleAnimInstance.h"
#include "MinorRoleAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class LBGC_API UMinorRoleAnimInstance : public UMainRoleAnimInstance
{
	GENERATED_BODY()
	
public:
	UMinorRoleAnimInstance();
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	UFUNCTION(BlueprintCallable, Category = "RoleAnimInstance  Func")
		void UpdateMinorRoleAnimationProperties(float fDeltaSeconds);

protected:
	virtual bool TraceCheckFootIK(EnFootIkTraceType type);

private:
	bool CheckMinorRole();

private:
	class AMinorRole* m_MinorRole;

};

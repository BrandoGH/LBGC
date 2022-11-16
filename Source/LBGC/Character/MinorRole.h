// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RoleBase.h"
#include "MinorRole.generated.h"

/**
 * 
 */
UCLASS()
class LBGC_API AMinorRole : public ARoleBase
{
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	AMinorRole();

public:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Login() override;

	void SetMoveVelocity(uint16 velocity);
	uint16 GetMoveVelocity();

	void SetJumping(bool bJump) { m_bJumping = bJump; }
	bool IsJumping() { return m_bJumping; }

private:
	bool m_bJumping;
};

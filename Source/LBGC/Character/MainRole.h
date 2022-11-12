// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RoleBase.h"
#include "MainRole.generated.h"

/**
 * 
 */
UCLASS()
class LBGC_API AMainRole : public ARoleBase
{
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	AMainRole();

public:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Login() override;
	virtual void Logout() override;

private:
	void InitWhenBeginPlay();

	void MoveControl(EAxis::Type type, float fValue);
	void OnMoveForward(float fValue);
	void OnMoveRight(float fValue);
	void OnCameraPitch(float fValue);
	void OnCameraYaw(float fValue);
	void OnSpeedUp();
	void OnSlowDown();
	void OnJump();
	void OnStopJumping();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AMainRole Var")
		class USpringArmComponent* CompSpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AMainRole Var")
		class UCameraComponent* CompCamera;

private:
	bool m_bSpeedUp;
};

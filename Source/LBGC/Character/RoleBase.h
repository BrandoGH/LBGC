// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RoleBase.generated.h"

UCLASS()
class LBGC_API ARoleBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARoleBase();

public:	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Login() {}
	virtual void Logout() {}

	void SetRoleName(const FString& name) { m_roleName = name; }
	FString GetRoleName() { return m_roleName; }
	void SetCreateModelFlag(bool bCreate) { m_bCreateModel = bCreate; }
	bool IsCreateModel() { return m_bCreateModel; }

	float GetMoveSpeed();

protected:
	void CollisionPresets();



protected:
	FString m_roleName;
	bool m_bCreateModel;

};

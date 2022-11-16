// Fill out your copyright notice in the Description page of Project Settings.

#ifdef LBGS_DEBUG
PRAGMA_DISABLE_OPTIMIZATION
#endif // LBGS_DEBUG
#include "MinorRole.h"
#include <AIController.h>
#include <GameFramework/CharacterMovementComponent.h>

AMinorRole::AMinorRole()
	: ARoleBase()
	, m_bJumping(false)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (GetMesh())
	{
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.f, 0.f));
	}

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->RotationRate = FRotator(0.f, 300.f, 0.f);
		GetCharacterMovement()->JumpZVelocity = 400.f;
		GetCharacterMovement()->GravityScale = 1.2f;
		GetCharacterMovement()->AirControl = 0.2f;
		GetCharacterMovement()->MaxWalkSpeed = 200.f;
		GetCharacterMovement()->MaxAcceleration = 768.f;
		GetCharacterMovement()->BrakingDecelerationWalking = 0.f;
	}

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

}

void AMinorRole::BeginPlay()
{
	Super::BeginPlay();
}

void AMinorRole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMinorRole::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AMinorRole::Login()
{
	SetCreateModelFlag(true);
}

void AMinorRole::SetMoveVelocity(uint16 velocity)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = velocity;
	}
}

uint16 AMinorRole::GetMoveVelocity()
{
	if (GetCharacterMovement())
	{
		return GetCharacterMovement()->MaxWalkSpeed;
	}
	return 0;
}

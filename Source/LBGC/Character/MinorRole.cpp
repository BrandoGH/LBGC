// Fill out your copyright notice in the Description page of Project Settings.


#include "MinorRole.h"

AMinorRole::AMinorRole()
	: ARoleBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (GetMesh())
	{
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.f, 0.f));
	}

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
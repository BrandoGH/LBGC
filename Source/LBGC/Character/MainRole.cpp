// Fill out your copyright notice in the Description page of Project Settings.


#include "MainRole.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

namespace
{
const float g_fSpringArmLength = 300.f;
const float g_fWalkSeepdDefault = 300.f;
const float g_fWalkSeepdUp = 600.f;

const FVector g_vecThirdView = FVector(0.f, 0.f, 63.f);
const FVector g_vecMeshDefault = FVector(0.f, 0.f, -88.f);
const FRotator g_rotMeshDefault = FRotator(0.f, -90.f, 0.f);
const FVector g_vecSocketOffset = FVector(0.f, 90.f, 30.f);
const FVector g_vecCameraLocDefault = FVector(4.f, -5.f, 19.f);
}

AMainRole::AMainRole() 
	: ARoleBase()
	, CompSpringArm(NULL)
	, CompCamera(NULL)
	, m_bSpeedUp(false)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	CompSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CompSpringArm"));
	CompSpringArm->SetupAttachment(GetRootComponent());
	CompSpringArm->TargetArmLength = g_fSpringArmLength;
	CompSpringArm->SetRelativeLocation(g_vecThirdView);
	CompSpringArm->bUsePawnControlRotation = true;
	CompSpringArm->SocketOffset = g_vecSocketOffset;
	CompSpringArm->bEnableCameraLag = true;
	CompSpringArm->CameraLagSpeed = 10.f;

	CompCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("CompCamera"));
	CompCamera->SetupAttachment(CompSpringArm, USpringArmComponent::SocketName);
	CompCamera->bUsePawnControlRotation = false;
	CompCamera->SetRelativeLocation(g_vecCameraLocDefault);

	if (GetMesh())
	{
		GetMesh()->SetRelativeLocationAndRotation(g_vecMeshDefault, g_rotMeshDefault);
	}
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void AMainRole::BeginPlay()
{
	Super::BeginPlay();

	InitWhenBeginPlay();
}

void AMainRole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMainRole::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Role move
	PlayerInputComponent->BindAxis("OnMoveForward", this, &AMainRole::OnMoveForward);
	PlayerInputComponent->BindAxis("OnMoveRight", this, &AMainRole::OnMoveRight);

	// Camera rotation
	PlayerInputComponent->BindAxis("OnCameraPitch", this, &AMainRole::OnCameraPitch);
	PlayerInputComponent->BindAxis("OnCameraYaw", this, &AMainRole::OnCameraYaw);

	// Jump
	PlayerInputComponent->BindAction("OnJump", IE_Pressed, this, &AMainRole::OnJump);
	PlayerInputComponent->BindAction("OnJump", IE_Released, this, &AMainRole::OnStopJumping);

	// Run
	PlayerInputComponent->BindAction("OnSpeedUp", IE_Pressed, this, &AMainRole::OnSpeedUp);
	PlayerInputComponent->BindAction("OnSpeedUp", IE_Released, this, &AMainRole::OnSlowDown);
}

void AMainRole::Login()
{
	SetCreateModelFlag(true);
}

void AMainRole::Logout()
{
	SetCreateModelFlag(false);
}

void AMainRole::InitWhenBeginPlay()
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->RotationRate = FRotator(0.f, 300.f, 0.f);
		GetCharacterMovement()->JumpZVelocity = 400.f;
		GetCharacterMovement()->GravityScale = 1.2f;
		GetCharacterMovement()->AirControl = 0.2f;
		GetCharacterMovement()->SetWalkableFloorAngle(46.f);
		GetCharacterMovement()->MaxWalkSpeed = g_fWalkSeepdDefault;
		GetCharacterMovement()->MaxAcceleration = 300.f;
	}

}

void AMainRole::MoveControl(EAxis::Type type, float fValue)
{
	if (!Controller)
	{
		return;
	}

	FRotator Rot = Controller->GetControlRotation();
	FRotator YawRot(0.f, Rot.Yaw, 0.f);
	FVector DirectionVec = FRotationMatrix(YawRot).GetUnitAxis(type);
	AddMovementInput(DirectionVec, fValue);
}

void AMainRole::OnMoveForward(float fValue)
{
	MoveControl(EAxis::X, fValue);
}

void AMainRole::OnMoveRight(float fValue)
{
	MoveControl(EAxis::Y, fValue);
}

void AMainRole::OnCameraPitch(float fValue)
{
	APawn::AddControllerPitchInput(fValue);
}

void AMainRole::OnCameraYaw(float fValue)
{
	APawn::AddControllerYawInput(fValue);
}

void AMainRole::OnSpeedUp()
{
	if (!GetCharacterMovement() || GetCharacterMovement()->IsFalling())
	{
		return;
	}

	GetCharacterMovement()->MaxWalkSpeed = g_fWalkSeepdUp;
	m_bSpeedUp = true;
}

void AMainRole::OnSlowDown()
{
	if (!GetCharacterMovement())
	{
		return;
	}

	GetCharacterMovement()->MaxWalkSpeed = g_fWalkSeepdDefault;
	m_bSpeedUp = false;
}

void AMainRole::OnJump()
{
	Super::Jump();

}

void AMainRole::OnStopJumping()
{
	Super::StopJumping();

}

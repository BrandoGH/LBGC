// Fill out your copyright notice in the Description page of Project Settings.


#include "RoleBase.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ARoleBase::ARoleBase()
	: m_bCreateModel(false)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*
	* Character does not rotate with the mouse
	*/
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// collision preset
	CollisionPresets();

	// Can be control role default
	AutoPossessPlayer = EAutoReceiveInput::Disabled;
}

// Called when the game starts or when spawned
void ARoleBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARoleBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARoleBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float ARoleBase::GetMoveSpeed()
{
	FVector velocity = GetVelocity();
	velocity.Z = 0;	
	return velocity.Size();
}

void ARoleBase::CollisionPresets()
{
	UCapsuleComponent* CompCapSule = GetCapsuleComponent();
	if (!CompCapSule)
	{
		return;
	}
	TEnumAsByte<enum ECanBeCharacterBase> BaseStepOn(ECanBeCharacterBase::ECB_Yes);
	CompCapSule->CanCharacterStepUpOn = BaseStepOn;
	CompCapSule->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	// Collision preset
	CompCapSule->SetCollisionObjectType(ECC_Pawn);
	CompCapSule->SetCollisionResponseToAllChannels(ECR_Overlap);
	CompCapSule->SetCollisionResponseToChannel(ECC_Camera, ECollisionResponse::ECR_Block);
	CompCapSule->SetCollisionResponseToChannel(ECC_WorldStatic, ECollisionResponse::ECR_Block);
	CompCapSule->SetCollisionResponseToChannel(ECC_WorldDynamic, ECollisionResponse::ECR_Block);
	CompCapSule->SetCollisionResponseToChannel(ECC_PhysicsBody, ECollisionResponse::ECR_Block);
}


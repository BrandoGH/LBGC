// Fill out your copyright notice in the Description page of Project Settings.


#include "LBGCGameInstance.h"

void ULBGCGameInstance::Init()
{
}

void ULBGCGameInstance::Shutdown()
{
}

ULBGCGameInstance* ULBGCGameInstance::instance = NULL;
ULBGCGameInstance* ULBGCGameInstance::GetInstance()
{
	if (GEngine)
	{
		FWorldContext* context = GEngine->GetWorldContextFromGameViewport(GEngine->GameViewport);
		if (context)
		{
			instance = Cast<ULBGCGameInstance>(context->OwningGameInstance);
		}
	}

	return instance;
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "LBGCGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class LBGC_API ULBGCGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void Shutdown() override;

public:
	UFUNCTION(BlueprintCallable, Category = "ULBGCGameInstance Func")
		static ULBGCGameInstance* GetInstance();

private:
	static ULBGCGameInstance* instance;
	
};

#define LBGC_INSTANCE ULBGCGameInstance::GetInstance()

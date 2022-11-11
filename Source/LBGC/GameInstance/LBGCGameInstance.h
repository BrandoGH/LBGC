// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "LBGCGameInstance.generated.h"

DECLARE_DELEGATE_OneParam(FAyncLoadLevelDelegate, bool /*ok*/);

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

	class UTcpClient* GetTcpClient(int ClientSeq = 0);
	void LoadMainGameLevel(const FAyncLoadLevelDelegate& callback);

	void PrintDebugMessageOnScreen(int32 Key, float TimeToDisplay, FColor DisplayColor, const FString& DebugMessage, bool bNewerOnTop = true, const FVector2D& TextScale = FVector2D::UnitVector);

public:
	UFUNCTION(BlueprintCallable, Category = "ULBGCGameInstance Func")
		static ULBGCGameInstance* GetInstance();

private:
	void DeleteTcpClient();
	void AyncLoadLevel(const FString& mapDir, const FString& mapName, const FAyncLoadLevelDelegate& dg);

private:
	static ULBGCGameInstance* instance;

	class UTcpClient* m_tcpClient;
};

#define LBGC_INSTANCE ULBGCGameInstance::GetInstance()

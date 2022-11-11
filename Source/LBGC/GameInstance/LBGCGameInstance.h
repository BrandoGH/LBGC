// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "../Character/MainRole.h"
#include "LBGCGameInstance.generated.h"

DECLARE_DELEGATE_OneParam(FAyncLoadLevelDelegate, bool /*ok*/);

struct RoleInfoParam
{
	RoleInfoParam()
	{
		Reset();
	}

	void Reset()
	{
		m_bCreateModel = false;
	}

	FString m_localRoleName;
	bool m_bCreateModel;
};

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

	void SetLocalRoleName(const FString& name) { m_roleInfo.m_localRoleName = name; }
	FString GetLocalRoleName() { return m_roleInfo.m_localRoleName; }
	void SetCreateModelFlag(bool bCreate) { m_roleInfo.m_bCreateModel = bCreate; }
	bool IsCreateModel() { return m_roleInfo.m_bCreateModel; }
	void CreateLocalRole();

public:
	UFUNCTION(BlueprintCallable, Category = "ULBGCGameInstance Func")
		static ULBGCGameInstance* GetInstance();

private:
	void DeleteTcpClient();
	void AyncLoadLevel(const FString& mapDir, const FString& mapName, const FAyncLoadLevelDelegate& dg);

public:
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ULBGCGameInstance Var")
		TSubclassOf<AMainRole> SpawnLocalRole;

private:
	static ULBGCGameInstance* instance;

	class UTcpClient* m_tcpClient;
	RoleInfoParam m_roleInfo;
};

#define LBGC_INSTANCE ULBGCGameInstance::GetInstance()

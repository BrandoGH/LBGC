// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "../Character/MainRole.h"
#include "../Character/MinorRole.h"
#include <AIController.h>
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
	float GetTickDeltaSeconds();

	void CreateLocalRole();
	AMinorRole* CreateRemoteRole(const FString& roleName);

	AMainRole* GetLocalRole();
	AMinorRole* GetMinorRole(const FString& roleName);

	FVector GetLocalRoleLocation();
	class APlayerController* GetLocalRoleController();

public:
	UFUNCTION(BlueprintCallable, Category = "ULBGCGameInstance Func")
		static ULBGCGameInstance* GetInstance();

private:
	void DeleteTcpClient();
	void AyncLoadLevel(const FString& mapDir, const FString& mapName, const FAyncLoadLevelDelegate& dg);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ULBGCGameInstance Var")
		TSubclassOf<AMainRole> SpawnLocalRole;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ULBGCGameInstance Var")
		TSubclassOf<AMinorRole> SpawnRemoteRole;

private:
	static ULBGCGameInstance* instance;

	class UTcpClient* m_tcpClient;

	TMap<FString, AMinorRole*> m_mapRoleNameToMinorRoleModel;

};

#define LBGC_INSTANCE ULBGCGameInstance::GetInstance()

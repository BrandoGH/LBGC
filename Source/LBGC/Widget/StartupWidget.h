// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StartupWidget.generated.h"

/**
 * 
 */
UCLASS()
class LBGC_API UStartupWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual bool Initialize() override;

	void SetTip(const FString& str);
	void SetWaitngShow(bool bShow);

private:
	void DefaultSetting();

private:
	class UTextBlock* m_textTip;
	class UThrobber* m_waiting;

};

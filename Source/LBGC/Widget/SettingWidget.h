// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SettingWidget.generated.h"

/**
 * 
 */
UCLASS()
class LBGC_API USettingWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	enum EnWindowsMode
	{
		Fullscreen,
		WindowedFullscreen,
		Windowed,
	};

	enum EnEffectQueality
	{
		Low,
		Medium,
		High,
		Epic,
		Cinematic,
	};


public:
	virtual bool Initialize() override;

	int32 GetWindowsMode();
	int32 GetEffectQuality();
	
private:
	void DefaultSetting();

private:
	class UComboBoxString* m_combWindowsMode;
	class UComboBoxString* m_combEffectQuality;
};

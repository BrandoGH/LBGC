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

public:
	virtual bool Initialize() override;

	int32 GetWindowsMode();
	
private:
	void DefaultSetting();

private:
	class UComboBoxString* m_combWindowsMode;
};

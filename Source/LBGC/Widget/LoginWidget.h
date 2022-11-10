// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoginWidget.generated.h"

/**
 * 
 */
UCLASS()
class LBGC_API ULoginWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual bool Initialize() override;

	void SetLoginButtonEnable(bool bEnable);
	void SetLoadingShow(bool bShow);
	void SetTip(const FString& str);
	FString GetRoleName();
	FString GetRolePassword();

private:
	void DefaultSetting();
	
private:
	class UButton* m_btLogin;
	class UCircularThrobber* m_loading;
	class UTextBlock* m_textTip;
	class UEditableTextBox* m_editRoleName;
	class UEditableTextBox* m_editRolePassword;
};

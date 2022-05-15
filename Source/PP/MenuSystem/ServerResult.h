// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PP/MenuSystem/MainMenu.h"
#include "ServerResult.generated.h"

/**
 * 
 */
UCLASS()
class PP_API UServerResult : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	class UButton* BTN;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_ServerName;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_UserName;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_Fraction;

	void SetUp(class UMainMenu* MainMenu, uint32 Index, FSessionData Session);

	void SetTextColor(FLinearColor Color);
	void SetButtonSelected(bool Value);

private:
	class UMainMenu* MainMenu;
	uint32 ButtonIndex = -1;

	bool bSelected;

	UFUNCTION()
	void Clicked();
	UFUNCTION()
	void Hover();
	UFUNCTION()
	void UnHover();

};
